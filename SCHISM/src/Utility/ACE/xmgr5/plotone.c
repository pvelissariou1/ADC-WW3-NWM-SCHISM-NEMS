/* $Id: plotone.c,v 1.6 2007/01/21 19:31:17 pturner Exp $

 * plotone.c - entry for graphics
 *
 */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <setjmp.h>
#include "globals.h"
#include "symdefs.h"
#include "noxprotos.h"

void drawsetboxplot(int gno, int setno);
void drawsetxypolar();
void drawflow(int gno, int setno, velocityp vp);
void drawfegrid(int gno, int setno);
void dovelocitylegend(int gno);
void drawtricolor(plotarr p, Isolparms ip);

int density_flag;		/* temp, until interface for density plots is
				 * completed */
int render_tri;		/* */

/*
 * Check for missing flag.
 */
int hasMissing(plotarr *p)
{
    return p->hasmissing;
}

/*
 * Missing data value.
 */
double missingValue(plotarr *p)
{
    return p->missing;
}

int isMissing(plotarr *p, double y)
{
    if (hasMissing(p) && y == missingValue(p)) {
	return 1;
    } else {
	return 0;
    }
}

void draw_polar_graph(int gno)
{
    int i;
    world w;
    view v;
    framep f;
    legend leg;
    labels lab;
    plotarr p;
    get_graph_world(gno, &w);
    get_graph_view(gno, &v);
    get_graph_labels(gno, &lab);
    get_graph_legend(gno, &leg);
    get_graph_framep(gno, &f);
    setclipping(TRUE);
    setfixedscale(v.xv1, v.yv1, v.xv2, v.yv2, &w.xg1, &w.yg1, &w.xg2, &w.yg2);
    w.xg1 = 4.0 * cos(5.0 * M_PI / 4.0);
    w.yg1 = 4.0 * sin(5.0 * M_PI / 4.0);
    w.xg2 = 4.0 * cos(M_PI / 4.0);
    w.yg2 = 4.0 * sin(M_PI / 4.0);
    defineworld(w.xg1, w.yg1, w.xg2, w.yg2, 0, 0);
    viewport(v.xv1, v.yv1, v.xv2, v.yv2);
    setcolor(1);
    my_circle((w.xg1 + w.xg2) / 2.0, (w.yg1 + w.yg2) / 2.0, (w.xg2 - w.xg1) / 2.0);
    for (i = 0; i < g[gno].maxplot; i++) {
	drawsetxypolar(g[gno].p[i], i);
    }
}

void plotone(int gno)
{
    int i;
    world w;
    view v;
    framep f;
    legend leg;
    labels lab;
    plotarr p;

    if (g[gno].type == POLAR) {
	draw_polar_graph(gno);
	return;
    }
    get_graph_world(gno, &w);
    get_graph_view(gno, &v);
    get_graph_framep(gno, &f);
    get_graph_labels(gno, &lab);
    get_graph_legend(gno, &leg);
    setclipping(TRUE);
    defineworld(w.xg1, w.yg1, w.xg2, w.yg2, islogx(gno), islogy(gno));
    viewport(v.xv1, v.yv1, v.xv2, v.yv2);
    if (debuglevel == 5) {
	printf("frame\n");
    }
    if (f.active == ON) {
	if (f.fillbg == ON) {
	    setcolor(f.bgcolor);
	    fillrectcolor(w.xg1, w.yg1, w.xg2, w.yg2);
	}
	boxplot(gno);
    }
    if (debuglevel == 5) {
	printf("axes\n");
    }
    setlinestyle(1);
    setlinewidth(1);
    drawaxes(gno);
    setlinestyle(1);
    setlinewidth(1);

    if (debuglevel == 5) {
	printf("title\n");
    }
    if (lab.title.s[0]) {
	if (debuglevel == 5) {
	    printf("draw title\n");
	}
	setlinewidth(lab.title.linew);
	setcolor(lab.title.color);
	setcharsize(lab.title.charsize);
	setfont(lab.title.font);
	drawtitle(lab.title.s, 0);
    }
    if (debuglevel == 5) {
	printf("subtitle\n");
    }
    if (lab.stitle.s[0]) {
	if (debuglevel == 5) {
	    printf("draw subtitle\n");
	}
	setlinewidth(lab.stitle.linew);
	setcolor(lab.stitle.color);
	setcharsize(lab.stitle.charsize);
	setfont(lab.stitle.font);
	drawtitle(lab.stitle.s, 1);
    }
    if (debuglevel == 5) {
	printf("fixed pt\n");
    }
    if (g[gno].pointset) {	/* mark the reference point */
	drawpolysym(&g[gno].dsx, &g[gno].dsy, 1, SYM_CIRCLE, 0, 0, 1.0);
	drawpolysym(&g[gno].dsx, &g[gno].dsy, 1, SYM_PLUS, 0, 0, 1.0);
    }
    if (debuglevel == 5) {
	printf("sets\n");
    }
    /*
     * draw sets
     */
    switch (g[gno].type) {
    case BAR:
	{
	    int tset = 0, maxn = 0;
	    double x1, y1, x2, y2, minx, miny, maxx, maxy;
	    double tdx, fmin(double x, double y), fmax(double x, double y);

	    minx = miny = maxx = maxy = 0.0;
	    for (i = 0; i < g[gno].maxplot; i++) {
		if (isactive(gno, i)) {
		    tset++;
		    maxn += getsetlength(gno, i);
		    getsetminmax(gno, i, &x1, &x2, &y1, &y2);
		    if (tset == 1) {
			minx = x1;
			miny = y1;
			maxx = x2;
			maxy = y2;
		    } else {
			minx = fmin(x1, minx);
			miny = fmin(y1, miny);
			maxx = fmax(x2, maxx);
			maxy = fmax(y2, maxy);
		    }
		}
	    }
	    tdx = maxx - minx;
	    if (tset != 0 && maxn != 0) {
		double offsx, offsy;
		double bsize = g[gno].barwid * tdx / maxn;
		double cset = -tset / 2.0;

		offsx = -cset * bsize - bsize / 2.0;
		offsy = 0.0;

		for (i = 0; i < g[gno].maxplot; i++) {

		    if (isactive(gno, i)) {
			drawsetbar(gno, i, cset, bsize);
			switch (dataset_type(gno, i)) {
			case XYDX:
			case XYDY:
			case XYDXDX:
			case XYDYDY:
			    drawseterrbar(gno, i, offsx, offsy);
			    break;
			case XYHILO:
			case XYBOXPLOT:
			case XYBOX:
			    break;
			case XYZ:
			    break;
			}
			cset += 1.0;
			offsx = -cset * bsize - bsize / 2.0;
			offsy = 0.0;
		    }
		}
	    }
	}
	break;
    case STACKEDBAR:
	{
	    int tset = 0, maxn = 0;
	    double x1, y1, x2, y2, minx, miny, maxx, maxy;
	    double tdx, fmin(double x, double y), fmax(double x, double y);

	    minx = miny = maxx = maxy = 0.0;
	    for (i = 0; i < g[gno].maxplot; i++) {
		if (isactive(gno, i)) {
		    tset++;
		    getsetminmax(gno, i, &x1, &x2, &y1, &y2);
		    if (tset == 1) {
			maxn = getsetlength(gno, i);
			minx = x1;
			miny = y1;
			maxx = x2;
			maxy = y2;
		    } else {
			maxn = (maxn < getsetlength(gno, i)) ? getsetlength(gno, i) : maxn;
			minx = fmin(x1, minx);
			miny = fmin(y1, miny);
			maxx = fmax(x2, maxx);
			maxy = fmax(y2, maxy);
		    }
		}
	    }
	    tdx = maxx - minx;
	    if (tset != 0 && maxn != 0) {
		double bsize = g[gno].sbarwid * tdx / maxn;

		drawsetstackedbar(gno, maxn, bsize);
	    }
	}
	break;
    case HBAR:
	{
	    int tset = 0, maxn = 0;
	    double x1, y1, x2, y2, minx, miny, maxx, maxy;
	    double tdy, fmin(double x, double y), fmax(double x, double y);

	    minx = miny = maxx = maxy = 0.0;
	    for (i = 0; i < g[gno].maxplot; i++) {
		if (isactive(gno, i)) {
		    tset++;
		    maxn += getsetlength(gno, i);
		    getsetminmax(gno, i, &x1, &x2, &y1, &y2);
		    if (tset == 1) {
			minx = x1;
			miny = y1;
			maxx = x2;
			maxy = y2;
		    } else {
			minx = fmin(x1, minx);
			miny = fmin(y1, miny);
			maxx = fmax(x2, maxx);
			maxy = fmax(y2, maxy);
		    }
		}
	    }
	    tdy = maxy - miny;
	    if (tset != 0 && maxn != 0) {
		double offsx, offsy;
		double bsize = g[gno].barwid * tdy / maxn;
		double cset = -tset / 2.0;

		offsy = -cset * bsize - bsize / 2.0;
		offsx = 0.0;

		for (i = 0; i < g[gno].maxplot; i++) {

		    if (isactive(gno, i)) {
			drawsethbar(gno, i, cset, bsize);
			switch (dataset_type(gno, i)) {
			case XYDX:
			case XYDY:
			case XYDXDX:
			case XYDYDY:
			    drawseterrbar(gno, i, offsx, offsy);
			    break;
			case XYHILO:
			case XYBOXPLOT:
			case XYBOX:
			    break;
			}
			cset += 1.0;
			offsy = -cset * bsize - bsize / 2.0;
			offsx = 0.0;
		    }
		}
	    }
	}
	break;
    case STACKEDHBAR:
	{
	    int tset = 0, maxn = 0;
	    double x1, y1, x2, y2, minx, miny, maxx, maxy;
	    double tdy, fmin(double x, double y), fmax(double x, double y);

	    minx = miny = maxx = maxy = 0.0;
	    for (i = 0; i < g[gno].maxplot; i++) {
		if (isactive(gno, i)) {
		    tset++;
		    getsetminmax(gno, i, &x1, &x2, &y1, &y2);
		    if (tset == 1) {
			maxn = getsetlength(gno, i);
			minx = x1;
			miny = y1;
			maxx = x2;
			maxy = y2;
		    } else {
			maxn = (maxn < getsetlength(gno, i)) ? getsetlength(gno, i) : maxn;
			minx = fmin(x1, minx);
			miny = fmin(y1, miny);
			maxx = fmax(x2, maxx);
			maxy = fmax(y2, maxy);
		    }
		}
	    }
	    tdy = maxy - miny;
	    if (tset != 0 && maxn != 0) {
		double bsize = g[gno].sbarwid * tdy / maxn;
		drawsetstackedhbar(gno, maxn, bsize);
	    }
	}
	break;
    case POLAR:
	break;
    case PIE:
	break;
    case STACKEDLINE:
	break;
    case LOGX:
    case LOGY:
    case LOGXY:
    case XY:
	for (i = 0; i < g[gno].maxplot; i++) {
	    get_graph_plotarr(gno, i, &p);
	    if (isactive(gno, i)) {
		switch (dataset_type(gno, i)) {
		case XY:
		    if (p.fill) {
			drawsetfill(gno, p);
		    }
		    drawsetxy(gno, g[gno].p[i], i);
		    break;
		case XYDX:
		case XYDY:
		case XYDXDX:
		case XYDYDY:
		case XYDXDY:
		    if (p.fill) {
			drawsetfill(gno, p);
		    }
		    drawseterrbar(gno, i, 0.0, 0.0);
		    drawsetxy(gno, g[gno].p[i], i);
		    break;
		case XYXX:
		    drawsetxy(gno, g[gno].p[i], i);
		    break;
		case XYYY:
		    drawsetxy(gno, g[gno].p[i], i);
		    break;
		case XYZ:
		    if (density_flag) {
			drawdensity(g[gno].p[i]);
		    } else if (render_tri) {
			drawtricolor(g[gno].p[i], g[gno].isol);
		    } else {
			drawsetxy(gno, g[gno].p[i], i);
			drawval(g[gno].p[i]);
		    }
		    break;
		case XYZW:
		    drawsetxy(gno, g[gno].p[i], i);
		    break;
		case XYRT:
		    drawsetxy(gno, g[gno].p[i], i);
		    drawcirclexy(g[gno].p[i]);
		    break;
		case XYX2Y2:
		    break;
		case XYSEG:
		    break;
		case XYBOX:
		    drawboxcolor(g[gno].p[i], g[gno].isol);
		    break;
		case XYBOXPLOT:
		    drawsetboxplot(gno, i);
		    break;
		case XYHILO:
		    drawsethilo(g[gno].p[i]);
		    break;
		case XYUV:
		    drawsetxy(gno, g[gno].p[i], i);
		    drawflow(gno, i, g[gno].vp);
		    break;
		case XYSTRING:
		    break;
		case FEGRID:
		    drawfegrid(gno, i);
		    break;
		}
	    }
	}
    }
    if (debuglevel == 5) {
	printf("regions\n");
    }
    setlinestyle(grdefaults.lines);
    setlinewidth(grdefaults.linew);
/*
 * draw any defined regions for this graph
 */
    for (i = 0; i < MAXREGION; i++) {
	if (rg[i].active == ON && rg[i].linkto[gno]) {
	    draw_region(i);
	}
    }

    if (debuglevel == 5) {
	printf("legend\n");
    }
    setlinestyle(grdefaults.lines);
    setlinewidth(grdefaults.linew);
    if (leg.active == ON) {
	dolegend(gno);
	setlinestyle(grdefaults.lines);
	setlinewidth(grdefaults.linew);
    }
    if (g[gno].vp.active == ON) {
	dovelocitylegend(gno);
    }
    if (timestamp.active == ON) {
	double xtmp, ytmp;
	if (debuglevel == 5) {
	    printf("draw timestamp\n");
	}
	set_timestamp();
	setlinewidth(timestamp.linew);
	setcolor(timestamp.color);
	setcharsize(timestamp.charsize);
	setfont(timestamp.font);
	view2world(timestamp.x, timestamp.y, &xtmp, &ytmp);
	writestr(xtmp, ytmp, timestamp.rot, timestamp.just, timestamp.s);
    }
}

void draw_ref_point(int gno)
{
    drawpolysym(&g[gno].dsx, &g[gno].dsy, 1, SYM_CIRCLE, 0, 0, 1.0);
    drawpolysym(&g[gno].dsx, &g[gno].dsy, 1, SYM_PLUS, 0, 0, 1.0);
    drawpolysym(&g[gno].dsx, &g[gno].dsy, 1, SYM_PLUS, 0, 0, 1.0);
}

void draw_annotation(int gno)
{
    int i;

    setclipping(0);		/* shut down clipping for strings, boxes,
				 * lines, and legends */
    if (debuglevel == 5) {
	printf("Boxes\n");
    }
    for (i = 0; i < MAXBOXES; i++) {
	if (isactive_box(i)) {
	    draw_box(gno, i);
	}
    }
    if (debuglevel == 5) {
	printf("Lines\n");
    }
    for (i = 0; i < MAXLINES; i++) {
	if (isactive_line(i)) {
	    draw_line(gno, i);
	}
    }
    if (debuglevel == 5) {
	printf("Strings\n");
    }
    for (i = 0; i < MAXSTR; i++) {
	if (isactive_string(i)) {
	    if (debuglevel == 5) {
		printf("String %d\n", i);
	    }
	    draw_string(gno, i);
	}
    }
    setclipping(1);
}

void dovelocitylegend(int gno)
{
    int c, s, wi;
    double tmpx, tmpy;
    velocityp vp = g[gno].vp;
    c = setcolor(vp.color);
    s = setlinestyle(vp.lines);
    wi = setlinewidth(vp.linew);
    if (vp.loctype == VIEW) {
	view2world(vp.velx, vp.vely, &tmpx, &tmpy);
    } else {
	tmpx = vp.velx;
	tmpy = vp.vely;
    }
    drawvellegend(tmpx, tmpy, vp.userlength, vp.vscale, vp.vstr.s, vp.arrowtype);
    setcolor(c);
    setlinestyle(s);
    setlinewidth(wi);
}

/*
 * draw the legend at (legx, legy)
 * ib = 1 in loop means accumulate info for box
 * ib = -1 after loop means draw box
 */
void dolegend(int gno)
{

    int i, j = 0, sy = 0, cy = 0, ly = 0, wy = 0, ib = 0;
    double tmpx, tmpy;
    legend l;
    plotarr p;

    get_graph_legend(gno, &l);

    if (l.loctype == VIEW) {
	view2world(l.legx, l.legy, &tmpx, &tmpy);
    } else {
	tmpx = l.legx;
	tmpy = l.legy;
    }
    j = 0;
    setcharsize(l.charsize);
    setfont(l.font);
    if (l.box == ON) {		/* compute a bounding box for the legend */
	for (i = 0; i < maxplot; i++) {
	    if (isactive(gno, i) && (l.str[i].s[0])) {
		get_graph_plotarr(gno, i, &p);
		putlegend(j, 1, l.len, l.vgap, p.symsize, tmpx, tmpy, sy, ly, cy, wy, l.str[i].s, p.symfill, -1, -1, -1);
		j++;
	    }
	}
	putlegendrect(l.boxfill == ON,
		      l.boxfillusing == COLOR,
		      l.boxfillcolor,
		      l.boxfillpat,
		      l.boxlcolor,
		      l.boxlinew,
		      l.boxlines);
    }
    j = 0;
    for (i = 0; i < maxplot; i++) {
	if (isactive(gno, i) && (l.str[i].s[0])) {
	    setcolor(l.color);
	    setcharsize(l.charsize);
	    setfont(l.font);
	    get_graph_plotarr(gno, i, &p);
	    sy = p.sym;
	    ly = p.lines;
	    wy = p.linew;
	    cy = p.color;
	    switch (g[gno].type) {
	    case XY:
		putlegend(j, 0, l.len, l.vgap, p.symsize,
			  tmpx, tmpy, sy, ly, cy, wy, l.str[i].s,
			  p.symfill, p.symcolor, p.symlinew, p.symlines);
		break;
	    case BAR:
	    case HBAR:
	    case STACKEDBAR:
	    case STACKEDHBAR:
		putbarlegend(j, 0, l.len, l.vgap, p.symsize,
			     tmpx, tmpy, sy, ly, cy, wy, l.str[i].s,
			     p.symfill, p.fillusing == PATTERN, p.fillcolor, p.fillpattern);
		break;
	    default:
		putlegend(j, 0, l.len, l.vgap, p.symsize,
			  tmpx, tmpy, sy, ly, cy, wy, l.str[i].s,
			  p.symfill, p.symcolor, p.symlinew, p.symlines);
		break;
	    }
	    j++;
	}
    }
}

/*
 * draw the graph frame
 */
void boxplot(int gno)
{
    world w, wtmp;
    framep f;
    int c, s, wi, type;
    double xt1, xt2, yt1, yt2;

    get_graph_world(gno, &wtmp);
    get_graph_framep(gno, &f);

    c = setcolor(f.color);
    s = setlinestyle(f.lines);
    wi = setlinewidth(f.linew);

    if (isxreversed(gno)) {
	w.xg2 = wtmp.xg1;
	w.xg1 = wtmp.xg2;
    } else {
	w.xg1 = wtmp.xg1;
	w.xg2 = wtmp.xg2;
    }

    if (isyreversed(gno)) {
	w.yg2 = wtmp.yg1;
	w.yg1 = wtmp.yg2;
    } else {
	w.yg1 = wtmp.yg1;
	w.yg2 = wtmp.yg2;
    }

    switch (f.type) {
    case 0:
	rect(w.xg1, w.yg1, w.xg2, w.yg2);
	break;
    case 1:			/* half open */
	my_move2(w.xg1, w.yg1);
	my_draw2(w.xg2, w.yg1);
	my_move2(w.xg1, w.yg1);
	my_draw2(w.xg1, w.yg2);
	break;
    case 2:			/* break top */
	my_move2(w.xg2, w.yg2);
	my_draw2(w.xg2, w.yg1);
	my_draw2(w.xg1, w.yg1);
	my_draw2(w.xg1, w.yg2);
	break;
    case 3:			/* break bottom */
	my_move2(w.xg1, w.yg1);
	my_draw2(w.xg1, w.yg2);
	my_draw2(w.xg2, w.yg2);
	my_draw2(w.xg2, w.yg1);
	break;
    case 4:			/* break left */
	my_move2(w.xg1, w.yg2);
	my_draw2(w.xg2, w.yg2);
	my_draw2(w.xg2, w.yg1);
	my_draw2(w.xg1, w.yg1);
	break;
    case 5:			/* break right */
	my_move2(w.xg2, w.yg2);
	my_draw2(w.xg1, w.yg2);
	my_draw2(w.xg1, w.yg1);
	my_draw2(w.xg2, w.yg1);
	break;
    }
    setcolor(c);
    setlinestyle(s);
    setlinewidth(wi);
}

/*
 * draw annotative text
 */
void draw_string(int gno, int i)
{
    double xtmp1, ytmp1;
    int f, c, w;
    double s;
    plotstr pstr;

    get_graph_string(i, &pstr);
    if (debuglevel == 5) {
	printf("String %d %s\n", i, pstr.s);
    }
    if (gno != -2) {
	if (pstr.loctype == WORLD && pstr.gno != gno) {
	    return;
	}
	if (pstr.loctype == VIEW && gno != -1) {
	    return;
	}
    }
    if (strlen(pstr.s) && (pstr.charsize > 0.0) && (pstr.active == ON)) {
	c = setcolor(pstr.color);
	w = setlinewidth(pstr.linew);
	s = setcharsize(pstr.charsize);
	f = setfont(pstr.font);
	if (pstr.loctype == WORLD) {
	    writestr(pstr.x, pstr.y, pstr.rot, pstr.just, pstr.s);
	} else {
	    view2world(pstr.x, pstr.y, &xtmp1, &ytmp1);
	    writestr(xtmp1, ytmp1, pstr.rot, pstr.just, pstr.s);
	}
	(void) setcolor(c);
	(void) setlinewidth(w);
	(void) setcharsize(s);
	(void) setfont(f);
    }
}

/*
 * draw annotative boxes
 */
void draw_box(int gno, int i)
{
    double xtmp1, ytmp1;
    double xtmp2, ytmp2;
    int c, l, w;
    boxtype b;

    get_graph_box(i, &b);
    if (gno != -2) {
	if (b.loctype == WORLD && b.gno != gno) {
	    return;
	}
	if (b.loctype == VIEW && gno != -1) {
	    return;
	}
    }
    if (b.active == ON) {
	setclipping(0);

	if (b.fill == COLOR) {
	    c = setcolor(b.fillcolor);
	    if (b.loctype == WORLD) {
		fillrectcolor(b.x1, b.y1, b.x2, b.y2);
	    } else {
		view2world(b.x1, b.y1, &xtmp1, &ytmp1);
		view2world(b.x2, b.y2, &xtmp2, &ytmp2);
		fillrectcolor(xtmp1, ytmp1, xtmp2, ytmp2);
	    }
	    setcolor(c);
	} else if (b.fill == PATTERN) {
	    c = setpattern(b.fillpattern);
	    if (b.loctype == WORLD) {
		fillrectpat(b.x1, b.y1, b.x2, b.y2);
	    } else {
		view2world(b.x1, b.y1, &xtmp1, &ytmp1);
		view2world(b.x2, b.y2, &xtmp2, &ytmp2);
		fillrectpat(xtmp1, ytmp1, xtmp2, ytmp2);
	    }
	}
	c = setcolor(b.color);
	l = setlinestyle(b.lines);
	w = setlinewidth(b.linew);
	if (b.loctype == WORLD) {
	    rect(b.x1, b.y1, b.x2, b.y2);
	} else {
	    view2world(b.x1, b.y1, &xtmp1, &ytmp1);
	    view2world(b.x2, b.y2, &xtmp2, &ytmp2);
	    rect(xtmp1, ytmp1, xtmp2, ytmp2);
	}
	setclipping(1);
	setcolor(c);
	setlinewidth(w);
	setlinestyle(l);
    }
}

/*
 * draw annotative lines
 */
void draw_line(int gno, int i)
{
    double xtmp1, ytmp1;
    double xtmp2, ytmp2;
    int c, ll, w;
    linetype l;

    get_graph_line(i, &l);
    if (gno != -2) {
	if (l.loctype == WORLD && l.gno != gno) {
	    return;
	}
	if (l.loctype == VIEW && gno != -1) {
	    return;
	}
    }
    if (l.active == ON) {
	setclipping(0);
	c = setcolor(l.color);
	ll = setlinestyle(l.lines);
	w = setlinewidth(l.linew);
	if (l.loctype == WORLD) {
	    draw_arrow(l.x1, l.y1, l.x2, l.y2, l.arrow, l.asize, l.atype);
	} else {
	    view2world(l.x1, l.y1, &xtmp1, &ytmp1);
	    view2world(l.x2, l.y2, &xtmp2, &ytmp2);
	    draw_arrow(xtmp1, ytmp1, xtmp2, ytmp2, l.arrow, l.asize, l.atype);
	}
	setclipping(1);
	setcolor(c);
	setlinewidth(w);
	setlinestyle(ll);
    }
}

/*
 * draw a set with a fill
 */
void drawsetfill(int gno, plotarr p)
{
    int i, c, len;
    double *x = p.ex[0], *y = p.ex[1], *xtmp, *ytmp, xb[4], yb[4];

    len = p.len + 2;
    xtmp = (double *) calloc(len, sizeof(double));
    ytmp = (double *) calloc(len, sizeof(double));
    if (xtmp == NULL || ytmp == NULL) {
	errwin("Can't malloc for fills in plotone");
	cxfree(xtmp);
	cxfree(ytmp);
	return;
    }
    for (i = 0; i < p.len; i++) {
	xtmp[i] = x[i];
	ytmp[i] = y[i];
    }
    switch (p.fill) {
    case 1:
	len = p.len;
	break;
    case 2:
	xtmp[p.len] = (p.xmax > g[gno].w.xg2) ? g[gno].w.xg2 : p.xmax;
	ytmp[p.len] = 0.0;
	xtmp[p.len + 1] = (p.xmin < g[gno].w.xg1) ? g[gno].w.xg1 : p.xmin;
	ytmp[p.len + 1] = 0.0;
	break;
    case 3:
	xtmp[p.len] = 0.0;
	ytmp[p.len] = (p.ymax > g[gno].w.yg2) ? g[gno].w.yg2 : p.ymax;
	xtmp[p.len + 1] = 0.0;
	ytmp[p.len + 1] = (p.ymin < g[gno].w.yg1) ? g[gno].w.yg1 : p.ymin;
	break;
    case 4:
	xtmp[p.len] = g[gno].w.xg1;
	ytmp[p.len] = (p.ymax > g[gno].w.yg2) ? g[gno].w.yg2 : p.ymax;
	xtmp[p.len + 1] = g[gno].w.xg1;
	ytmp[p.len + 1] = (p.ymin < g[gno].w.yg1) ? g[gno].w.yg1 : p.ymin;
	break;
    case 5:
	xtmp[p.len] = g[gno].w.xg2;
	ytmp[p.len] = (p.ymax > g[gno].w.yg2) ? g[gno].w.yg2 : p.ymax;
	xtmp[p.len + 1] = g[gno].w.xg2;
	ytmp[p.len + 1] = (p.ymin < g[gno].w.yg1) ? g[gno].w.yg1 : p.ymin;
	break;
    case 6:			/* fill to ymin */
	xtmp[p.len] = (p.xmax > g[gno].w.xg2) ? g[gno].w.xg2 : p.xmax;
	ytmp[p.len] = g[gno].w.yg1;
	xtmp[p.len + 1] = (p.xmin < g[gno].w.xg1) ? g[gno].w.xg1 : p.xmin;
	ytmp[p.len + 1] = g[gno].w.yg1;
	break;
    case 7:
	xtmp[p.len] = (p.xmax > g[gno].w.xg2) ? g[gno].w.xg2 : p.xmax;
	ytmp[p.len] = g[gno].w.yg2;
	xtmp[p.len + 1] = (p.xmin < g[gno].w.xg1) ? g[gno].w.xg1 : p.xmin;
	ytmp[p.len + 1] = g[gno].w.yg2;
	break;
    }
    if (p.fillusing == COLOR) {
	c = setcolor(p.fillcolor);
	fillcolor(len, xtmp, ytmp);
	setcolor(c);
    } else if (p.fillusing == PATTERN) {
	setpattern(p.fillpattern);
	fillpattern(len, xtmp, ytmp);
    }
    cxfree(xtmp);
    cxfree(ytmp);
}

/*
 * draw a standard set with symbols and lines
 */
void drawsetxy(int gno, plotarr p, int i)
{
    int j, c, l, w, cy = p.color, sy = p.sym, ly = p.lines, wy = p.linew;
    double xbar, sd, *x = p.ex[0], *y = p.ex[1];
    char s[256];
    int save = 0;

    getlineprops(&c, &l, &w);
/*
   printf("%d %d %d %d %d %d\n", c, l, w, cy, ly, wy);
 */
/* draw the line */
    if (wy && ly) {
	if (cy != c) {
	    setcolor(cy);
	}
	if (wy != w) {
	    setlinewidth(wy);
	}
	if (ly != l) {
	    setlinestyle(ly);
	}
	save = 1;
        if (p.hasmissing) {
	    drawpolymissing(p.missing,x, y, p.len);
	} else {
	    drawpoly(x, y, p.len);
	}
    }
/* draw the symbol */
    if (sy && p.symlinew != 0 && p.symlines != 0) {
	sd = setcharsize(p.symsize);
	if (p.symcolor != c) {
	    setcolor(p.symcolor);
	}
	setlinewidth(p.symlinew);
	setlinestyle(p.symlines);
	save = 1;
	switch (sy) {
	case SYM_SEG:		/* draw segments */
	    drawpolyseg(x, y, p.len);
	    break;
	case SYM_CHAR:		/* draw character */
	    setfont(p.font);
	    if (p.symchar > ' ') {
		int skip = p.symskip + 1;

		s[0] = p.symchar;
		s[1] = 0;
		for (j = 0; j < p.len; j += skip) {
		    if (symok(x[j], y[j])) {
			writestr(x[j], y[j], 0, 2, s);
		    }
		}
	    }
	    break;
	case SYM_HILOX:	/* draw hilo along X */
	    if (p.ex[2] != NULL) {
		for (j = 0; j < p.len; j++) {
		    my_move2(x[j], y[j]);
		    my_draw2(p.ex[2][j], y[j]);
		}
	    }
	    break;
	case SYM_HILOY:	/* draw hilo along Y */
	    if (p.ex[2] != NULL) {
		for (j = 0; j < p.len; j++) {
		    my_move2(x[j], y[j]);
		    my_draw2(x[j], p.ex[2][j]);
		}
	    }
	    break;
	case SYM_OPENCLOSEX:	/* draw open/close along X */
	    if (p.ex[2] != NULL) {
		for (j = 0; j < p.len; j++) {
		    openclose(y[j], x[j], p.ex[2][j], 1.0, 0);
		}
	    }
	    break;
	case SYM_OPENCLOSEY:	/* draw open/close along Y */
	    if (p.ex[2] != NULL) {
		for (j = 0; j < p.len; j++) {
		    openclose(x[j], y[j], p.ex[2][j], 1.0, 1);
		}
	    }
	    break;
	case SYM_RANGE:	/* draw bounding box */
	    rect(p.xmin, p.ymin, p.xmax, p.ymax);
	    stasum(y, p.len, &xbar, &sd, 0);
	    my_move2(p.xmin, xbar);
	    my_draw2(p.xmax, xbar);
	    stasum(x, p.len, &xbar, &sd, 0);
	    my_move2(xbar, p.ymin);
	    my_draw2(xbar, p.ymax);
	    break;
	case SYM_TAG_FIRST:	/* tag the first point in a set */
	    if (symok(x[0], y[0])) {
		sprintf(s, "S%1d:1", i);
		writestr(x[0], y[0], 0, 2, s);
	    }
	    break;
	case SYM_TAG_LAST:	/* tag the last point in a set */
	    if (symok(x[p.len - 1], y[p.len - 1])) {
		sprintf(s, "S%1d:%1d", i, p.len);
		writestr(x[p.len - 1], y[p.len - 1], 0, 2, s);
	    }
	    break;
	case SYM_TAG_CENTER:	/* tag the middle point in a set */
	    if (symok(x[p.len / 2], y[p.len / 2])) {
		sprintf(s, "S%1d:%1d", i, p.len / 2);
		writestr(x[p.len / 2], y[p.len / 2], 0, 2, s);
	    }
	    break;
	case SYM_STRING:	/* string at plot */
	    /* drawpolystring(x, y, p.len, sy, 0); */
	    break;
	case SYM_SETNO_LOC:	/* set number and location */
	case SYM_SETNO:	/* set number */
	    for (j = 0; j < p.len; j++) {
		if (symok(x[j], y[j])) {
		    if (sy == SYM_SETNO) {
			sprintf(s, "S%d", i);
		    } else {
			sprintf(s, "S%1d:%1d)", i, j + 1);
		    }
		    writestr(x[j], y[j], 0, 0, s);
		}
	    }
	    break;
	case SYM_AVGY:		/* average Y */
	    stasum(y, p.len, &xbar, &sd, 0);
	    my_move2(g[gno].w.xg1, xbar);
	    my_draw2(g[gno].w.xg2, xbar);
	    break;
	case SYM_AVGSTDY1:
	    stasum(y, p.len, &xbar, &sd, 0);
	    my_move2(g[gno].w.xg1, xbar - sd);
	    my_draw2(g[gno].w.xg2, xbar - sd);
	    my_move2(g[gno].w.xg1, xbar + sd);
	    my_draw2(g[gno].w.xg2, xbar + sd);
	    break;
	case SYM_AVGSTDY2:
	    stasum(y, p.len, &xbar, &sd, 0);
	    my_move2(g[gno].w.xg1, xbar - 2.0 * sd);
	    my_draw2(g[gno].w.xg2, xbar - 2.0 * sd);
	    my_move2(g[gno].w.xg1, xbar + 2.0 * sd);
	    my_draw2(g[gno].w.xg2, xbar + 2.0 * sd);
	    break;
	case SYM_AVGSTDY3:
	    stasum(y, p.len, &xbar, &sd, 0);
	    my_move2(g[gno].w.xg1, xbar - 3.0 * sd);
	    my_draw2(g[gno].w.xg2, xbar - 3.0 * sd);
	    my_move2(g[gno].w.xg1, xbar + 3.0 * sd);
	    my_draw2(g[gno].w.xg2, xbar + 3.0 * sd);
	    break;
	case SYM_MEDIANY:
	    break;
	case SYM_GEOMMEANY:
	    break;
	case SYM_HARMMEANY:
	    break;
	default:
	    drawpolysym(x, y, p.len, sy, p.symskip, p.symfill, p.symsize);
	    break;
	}
	setcharsize(sd);
    }
    if (save) {
	setlinewidth(w);
	setlinestyle(l);
	setcolor(c);
    }
}

/*
 * draw hi/lo-open/close
 */
void drawsethilo(plotarr p)
{
    int i, c, w, cy = p.color, wy = p.linew;
    double *x = p.ex[0], *y = p.ex[1], *dx = p.ex[2], *dy = p.ex[3],
    *dz = p.ex[4];

    c = setcolor(cy);
    w = setlinewidth(wy);
    for (i = 0; i < p.len; i++) {
	my_move2(x[i], y[i]);
	my_draw2(x[i], dx[i]);
	openclose(x[i], dy[i], dz[i], 1.0, 1);
    }
    setlinewidth(w);
    setcolor(c);
}

/*
 * draw a value as a string at x, y
 */
void drawval(plotarr p)
{
    int j;
    double *x = p.ex[0], *y = p.ex[1], *z = p.ex[2], loc, ss;
    char *s1, s2[256];

    s2[0] = ' ';
    s1 = &s2[1];
    ss = setcharsize(p.symsize);
    setfont(p.font);
    for (j = 0; j < p.len; j++) {
	if (symok(x[j], y[j])) {
	    loc = z[j];
	    create_ticklabel(p.format, p.prec, loc, s1);
	    if (p.sym) {
		writestr(x[j], y[j], 0, 0, s1 - 1);
	    } else {
		writestr(x[j], y[j], 0, 2, s1);
	    }
	}
    }
    setcharsize(ss);
}

/*
 * draw a density plot
 */
void drawdensity(plotarr p)
{
    int j, c, w;
    double *x = p.ex[0], *y = p.ex[1], *z = p.ex[2], loc;
    char *s1, s2[256];

    c = setcolor(p.color);
    w = setlinewidth(p.linew);
    for (j = 0; j < p.len; j++) {
	if (symok(x[j], y[j])) {
	    loc = z[j];
	    if ((p.zmin == p.zmax) || (loc >= p.zmin && loc <= p.zmax)) {
		drawpolysym(&x[j], &y[j], 1, p.sym, p.symskip, p.symfill, z[j]);
	    }
	}
    }
    setcolor(c);
    setlinewidth(w);
}

/*
 * draw a rectangle with color
 */
void doisollegend(Isolparms ip, int *cmap, int nmap);
void drawiso(double x1, double y1, double x2, double y2, double x3, double y3, double c1, double c2, double c3, int imin, int imax, Isolparms ip);

void drawisolines(double x1, double y1, double x2, double y2, 
		double x3, double y3, double x4, double y4, 
		double d1, double d2, double d3, double d4, Isolparms ip)
{
    drawiso(x1, y1, x2, y2, x3, y3, d1, d2, d3, 0, ip.nisol, ip);
    drawiso(x1, y1, x3, y3, x4, y4, d1, d3, d4, 0, ip.nisol, ip);
}

/*
 *  Draw the isolines
 */
void drawiso(double x1, double y1, double x2, double y2, double x3, double y3, double c1, double c2, double c3, int imin, int imax, Isolparms ip)
{
    int i, k, l = 0, cnt = 0;
    int n1, n2, n3;
    double cc;
    double px[3], py[3];
    char buf[80];

    for (k = 0; k < ip.nisol; k++) {
	cc = ip.cis[k];
	l = 0;
	/* crosses at n1 n2 */
	if ((c1 >= cc && c2 <= cc) || (c1 <= cc && c2 >= cc)) {
	    if (c1 != c2) {
		px[l] = (cc - c1) * (x2 - x1) / (c2 - c1) + x1;
		py[l++] = (cc - c1) * (y2 - y1) / (c2 - c1) + y1;
	    }
	}
	/* crosses at n1 n3 */
	if ((c1 >= cc && c3 <= cc) || (c1 <= cc && c3 >= cc)) {
	    if (c1 != c3) {
		px[l] = (cc - c1) * (x3 - x1) / (c3 - c1) + x1;
		py[l++] = (cc - c1) * (y3 - y1) / (c3 - c1) + y1;
	    }
	}
	/* crosses at n2 n3 */
	if ((c2 >= cc && c3 <= cc) || (c2 <= cc && c3 >= cc)) {
	    if (c2 != c3) {
		px[l] = (cc - c2) * (x3 - x2) / (c3 - c2) + x2;
		py[l++] = (cc - c2) * (y3 - y2) / (c3 - c2) + y2;
	    }
	}
	/* setcolor(ip.color[k]); */
	setcolor(k + 6);
	setlinestyle(ip.lines[k]);
	setlinewidth(ip.linew[k]);
	if (l == 2) {
	    my_move2(px[0], py[0]);
	    my_draw2(px[1], py[1]);
	    cnt++;
	}
	if (l == 3) {
	    my_move2(px[0], py[0]);
	    my_draw2(px[1], py[1]);
	    my_draw2(px[2], py[2]);
	}
    }
}

void drawboxcolor(plotarr p, Isolparms ip)
{
    int i, j, k, c, w, cset;
    double tmpx, tmpy, dir;
    double *x1 = p.ex[0], *y1 = p.ex[1], loc;
    double *x2 = p.ex[2], *y2 = p.ex[3], *z = p.ex[4];
    char *s1, s2[256];
    extern int *colors;

    c = setcolor(p.color);
    w = setlinewidth(p.linew);
    for (j = 0; j < p.len - 1; j++) {
	if (z[j] < ip.cis[0] || z[j] > ip.cis[ip.nisol - 1]) {
	    continue;
	} else {
	    for (k = 1; k < ip.nisol; k++) {
		if (z[j] >= ip.cis[k - 1] && z[j] < ip.cis[k]) {
		    cset = k + 16;
		    break;
		}
	    }
	    setcolor(cset);
	    fillrectcolor(x1[j], y1[j], x2[j], y2[j]);
	}
    }
    setcolor(c);
    setlinewidth(w);
    if (ip.lactive == ON) {
	doisollegend(ip, colors, maxcolors);
    }
}

void drawtricolor(plotarr p, Isolparms ip)
{
    int i, j, k;
    double tmpx, tmpy, dir;
    double *x1 = p.ex[0], *y1 = p.ex[1], loc;
    double *z1 = p.ex[2];
    char *s1, s2[256];
    extern int *colors;
    double x[3], y[3], z[3];
    int c[3];
    int found;
    for (j = 0; j < p.len / 3; j++) {
	x[0] = x1[j * 3];
	x[1] = x1[1 + j * 3];
	x[2] = x1[2 + j * 3];
	y[0] = y1[j * 3];
	y[1] = y1[1 + j * 3];
	y[2] = y1[2 + j * 3];
	z[0] = z1[j * 3];
	z[1] = z1[1 + j * 3];
	z[2] = z1[2 + j * 3];
	found = 0;
	for (i = 0; i< 3; i++) {
	    if (z[i] < ip.cis[0] || z[i] > ip.cis[ip.nisol - 1]) {
	        continue;
	    } else {
	        for (k = 1; k < ip.nisol; k++) {
		    if (z[i] >= ip.cis[k - 1] && z[i] < ip.cis[k]) {
		        c[i] = k + 16;
			found++;
		        break;
		    }
	        }
	    }
	}
	if (found == 3) {
	     filltricolor(x, y, c);
	     found = 0;
	}
    }
    if (ip.lactive == ON) {
	doisollegend(ip, colors, maxcolors);
    }
}

void doisollegend(Isolparms ip, int *cmap, int nmap)
{
    double x, y;
    int f;
    switch (ip.p.format) {
    case DECIMAL:
	f = 0;
	break;
    case EXPONENTIAL:
	f = 1;
	break;
    case GENERAL:
	f = 2;
	break;
    }
    if (ip.loctype == WORLD) {
	x = ip.x;
	y = ip.y;
    } else {
	view2world(ip.x, ip.y, &x, &y);
    }
    if (ip.layout == HORIZONTAL) {
	drawisollegend(1, x, y, ip.cis, ip.nisol,
		       ip.xlen, ip.ylen, ip.xgap, ip.ygap, ip.p.charsize,
		       f, ip.p.prec, ip.p.color,
		       cmap, nmap, 0);
    } else {
	drawisollegend(0, x, y, ip.cis, ip.nisol,
		       ip.xlen, ip.ylen, ip.xgap, ip.ygap, ip.p.charsize,
		       f, ip.p.prec, ip.p.color,
		       cmap, nmap, 0);
    }
}

/*
 * draw a specified symbol at a point with color and size
 */
void drawsymcolor(plotarr p)
{
    int j, c, w, cset;
    double *x1 = p.ex[0], *y1 = p.ex[1], loc;
    double *x2 = p.ex[2], *y2 = p.ex[3], *z = p.ex[4];
    char *s1, s2[256];

    c = setcolor(p.color);
    w = setlinewidth(p.linew);
    for (j = 0; j < p.len; j++) {
	cset = (int) y2[j];
	setcolor(cset);
	c = setcolor(cset);
	drawpolysym(&x1[j], &y1[j], 1, cset, p.symskip, p.symfill, p.symsize);
    }
    setcolor(c);
    setlinewidth(w);
}


/*
 * draw a circle centered at x, y with radius z
 */
void drawcirclexy(plotarr p)
{
    int j;
    int cy = p.color, wy = p.linew;
    double *x = p.ex[0], *y = p.ex[1], *z = p.ex[2];

    cy = setcolor(cy);
    wy = setlinewidth(wy);
    for (j = 0; j < p.len; j++) {
	if (symok(x[j], y[j])) {
	    drawcircle(x[j], y[j], z[j], 0);
	}
    }
    setcolor(cy);
    setlinewidth(wy);
}

/*
 * draw a set in a bar chart
 */
void drawsetbar(int gno, int setno, double cset, double bsize)
{
    int i, j;
    int c, l, w, p, ls;
    int cc = g[gno].p[setno].color;
    int cy = g[gno].p[setno].fillcolor;
    int py = g[gno].p[setno].fillpattern;
    int ly = g[gno].p[setno].lines, wy = g[gno].p[setno].linew;
    double *x = getx(gno, setno), *y = gety(gno, setno);
    double tmpx[4];
    double tmpy[4];

    c = setcolor(cy);
    p = setpattern(py);
    l = setlinestyle(ly);
    w = setlinewidth(wy);
    if (g[gno].p[setno].fill) {
	for (i = 0; i < g[gno].p[setno].len; i++) {
	    tmpx[0] = x[i] + cset * bsize;
	    tmpy[0] = 0.0;
	    tmpx[1] = x[i] + cset * bsize;
	    tmpy[1] = y[i];
	    tmpx[2] = x[i] + (cset + 1.0) * bsize;
	    tmpy[2] = y[i];
	    tmpx[3] = x[i] + (cset + 1.0) * bsize;
	    tmpy[3] = 0.0;
	    if (tmpx[0] > g[gno].w.xg2) {
		continue;
	    }
	    if (tmpx[2] < g[gno].w.xg1) {
		continue;
	    }
	    for (j = 0; j < 4; j++) {
		if (tmpx[j] < g[gno].w.xg1)
		    tmpx[j] = g[gno].w.xg1;
		else if (tmpx[j] > g[gno].w.xg2)
		    tmpx[j] = g[gno].w.xg2;
		if (tmpy[j] < g[gno].w.yg1)
		    tmpy[j] = g[gno].w.yg1;
		else if (tmpy[j] > g[gno].w.yg2)
		    tmpy[j] = g[gno].w.yg2;
	    }
	    if (g[gno].p[setno].fillusing == PATTERN) {
		fillrectpat(tmpx[0], tmpy[0], tmpx[2], tmpy[2]);
	    } else if (g[gno].p[setno].fillusing == COLOR) {
		fillrectcolor(tmpx[0], tmpy[0], tmpx[2], tmpy[2]);
	    }
	    if (dataset_type(gno, setno) == XYZ) {
		int j;
		double *x = g[gno].p[setno].ex[0], *y = g[gno].p[setno].ex[1],
		*z = g[gno].p[setno].ex[2], loc, ss;
		char *s1, s2[256];

		s2[0] = ' ';
		s1 = &s2[1];
		ss = setcharsize(g[gno].p[setno].symsize);
		setfont(g[gno].p[setno].font);
		for (j = 0; j < g[gno].p[setno].len; j++) {
		    if (symok(x[j], y[j])) {
			loc = z[j];
			create_ticklabel(g[gno].p[setno].format, g[gno].p[setno].prec, loc, s1);
			if (g[gno].p[setno].sym) {
			    writestr(x[j] + (cset + 0.5) * bsize * 0.5, y[j], 0, 0, s1 - 1);
			} else {
			    writestr(x[j] + (cset + 0.5) * bsize * 0.5, y[j], 0, 2, s1);
			}
		    }
		}
		setcharsize(ss);
	    }
	}
    }
    if (ly && wy) {
	for (i = 0; i < g[gno].p[setno].len; i++) {
	    tmpx[0] = x[i] + cset * bsize;
	    tmpy[0] = 0.0;
	    tmpx[1] = x[i] + cset * bsize;
	    tmpy[1] = y[i];
	    tmpx[2] = x[i] + (cset + 1.0) * bsize;
	    tmpy[2] = y[i];
	    tmpx[3] = x[i] + (cset + 1.0) * bsize;
	    tmpy[3] = 0.0;
	    setcolor(cc);
	    setlinestyle(ly);
	    setlinewidth(wy);
	    my_move2(tmpx[0], tmpy[0]);
	    for (j = 0; j < 4; j++) {
		my_draw2(tmpx[(j + 1) % 4], tmpy[(j + 1) % 4]);
	    }
	}
    }
    setlinestyle(l);
    setlinewidth(w);
    setcolor(c);
    setpattern(p);
}

/*
 * draw a set in a horizontal bar chart
 */
void drawsethbar(int gno, int setno, double cset, double bsize)
{
    int i, j;
    int c, l, w, p, ls;
    int cc = g[gno].p[setno].color;
    int cy = g[gno].p[setno].fillcolor;
    int py = g[gno].p[setno].fillpattern;
    int ly = g[gno].p[setno].lines, wy = g[gno].p[setno].linew;
    double *x = getx(gno, setno), *y = gety(gno, setno);
    double tmpx[4];
    double tmpy[4];

    c = setcolor(cy);
    p = setpattern(py);
    l = setlinestyle(ly);
    w = setlinewidth(wy);
    if (g[gno].p[setno].fill) {
	for (i = 0; i < g[gno].p[setno].len; i++) {
	    tmpy[0] = y[i] + cset * bsize;
	    tmpx[0] = 0.0;
	    tmpy[1] = y[i] + cset * bsize;
	    tmpx[1] = x[i];
	    tmpy[2] = y[i] + (cset + 1.0) * bsize;
	    tmpx[2] = x[i];
	    tmpy[3] = y[i] + (cset + 1.0) * bsize;
	    tmpx[3] = 0.0;
	    if (tmpy[0] > g[gno].w.yg2) {
		continue;
	    }
	    if (tmpy[2] < g[gno].w.yg1) {
		continue;
	    }
	    for (j = 0; j < 4; j++) {
		if (tmpy[j] < g[gno].w.yg1)
		    tmpy[j] = g[gno].w.yg1;
		else if (tmpy[j] > g[gno].w.yg2)
		    tmpy[j] = g[gno].w.yg2;
		if (tmpx[j] < g[gno].w.xg1)
		    tmpx[j] = g[gno].w.xg1;
		else if (tmpx[j] > g[gno].w.xg2)
		    tmpx[j] = g[gno].w.xg2;
	    }
	    if (g[gno].p[setno].fillusing == PATTERN) {
		fillrectpat(tmpx[0], tmpy[0], tmpx[2], tmpy[2]);
	    } else if (g[gno].p[setno].fillusing == COLOR) {
		fillrectcolor(tmpx[0], tmpy[0], tmpx[2], tmpy[2]);
	    }
	}
    }
    if (ly && wy) {
	for (i = 0; i < g[gno].p[setno].len; i++) {
	    tmpy[0] = y[i] + cset * bsize;
	    tmpx[0] = 0.0;
	    tmpy[1] = y[i] + cset * bsize;
	    tmpx[1] = x[i];
	    tmpy[2] = y[i] + (cset + 1.0) * bsize;
	    tmpx[2] = x[i];
	    tmpy[3] = y[i] + (cset + 1.0) * bsize;
	    tmpx[3] = 0.0;
	    setcolor(cc);
	    setlinestyle(ly);
	    setlinewidth(wy);
	    my_move2(tmpx[0], tmpy[0]);
	    for (j = 0; j < 4; j++) {
		my_draw2(tmpx[(j + 1) % 4], tmpy[(j + 1) % 4]);
	    }
	}
    }
    setlinestyle(l);
    setlinewidth(w);
    setcolor(c);
    setpattern(p);
}

/*
 * draw a set in a stacked bar chart
 */
void drawsetstackedbar(int gno, int maxn, double bsize)
{
    int i, j, k;
    int c, l, w, p, ls;
    int cc, cy, py, ly, wy;
    double *x, *y;
    double tmpx[4];
    double tmpy[4];
    double *sum = (double *) calloc(maxn, sizeof(double));

    if (sum == NULL) {
	errwin("Can't calloc in drawsetstackedbar\n");
	return;
    }
    for (i = 0; i < g[gno].maxplot; i++) {
	if (isactive(gno, i)) {
	    x = getx(gno, i);
	    y = gety(gno, i);
	    cc = g[gno].p[i].color;
	    cy = g[gno].p[i].fillcolor;
	    py = g[gno].p[i].fillpattern;
	    ly = g[gno].p[i].lines;
	    wy = g[gno].p[i].linew;
	    c = setcolor(cy);
	    p = setpattern(py);
	    l = setlinestyle(ly);
	    w = setlinewidth(wy);
	    for (j = 0; j < maxn; j++) {
		if (j < getsetlength(gno, i)) {
		    if (g[gno].p[i].fill) {
			c = setcolor(cy);
			tmpx[0] = x[j] - bsize * 0.5;
			tmpy[0] = sum[j];
			tmpx[1] = x[j] - bsize * 0.5;
			tmpy[1] = sum[j] + y[j];
			tmpx[2] = x[j] + bsize * 0.5;
			tmpy[2] = sum[j] + y[j];
			tmpx[3] = x[j] + bsize * 0.5;
			tmpy[3] = sum[j];
			if (tmpx[0] > g[gno].w.xg2) {
			    continue;
			}
			if (tmpx[2] < g[gno].w.xg1) {
			    continue;
			}
			for (k = 0; k < 4; k++) {
			    if (tmpx[k] < g[gno].w.xg1)
				tmpx[k] = g[gno].w.xg1;
			    else if (tmpx[k] > g[gno].w.xg2)
				tmpx[k] = g[gno].w.xg2;
			    if (tmpy[k] < g[gno].w.yg1)
				tmpy[k] = g[gno].w.yg1;
			    else if (tmpy[k] > g[gno].w.yg2)
				tmpy[k] = g[gno].w.yg2;
			}
			if (g[gno].p[i].fillusing == PATTERN) {
			    fillrectpat(tmpx[0], tmpy[0], tmpx[2], tmpy[2]);
			} else if (g[gno].p[i].fillusing == COLOR) {
			    fillrectcolor(tmpx[0], tmpy[0], tmpx[2], tmpy[2]);
			}
		    }
		    if (ly && wy) {
			tmpx[0] = x[j] - bsize * 0.5;
			tmpy[0] = sum[j];
			tmpx[1] = x[j] - bsize * 0.5;
			tmpy[1] = sum[j] + y[j];
			tmpx[2] = x[j] + bsize * 0.5;
			tmpy[2] = sum[j] + y[j];
			tmpx[3] = x[j] + bsize * 0.5;
			tmpy[3] = sum[j];
			setcolor(cc);
			my_move2(tmpx[0], tmpy[0]);
			for (k = 0; k < 4; k++) {
			    my_draw2(tmpx[(k + 1) % 4], tmpy[(k + 1) % 4]);
			}
		    }
		    sum[j] += y[j];
		}
	    }
	}
    }
    free(sum);
}

/*
 * draw a set in a horizontal stacked bar chart
 */
void drawsetstackedhbar(int gno, int maxn, double bsize)
{
    int i, j, k;
    int c, l, w, p, ls;
    int cc, cy, py, ly, wy;
    double *x, *y;
    double tmpx[4];
    double tmpy[4];
    double *sum = (double *) calloc(maxn, sizeof(double));

    if (sum == NULL) {
	errwin("Can't calloc in drawsetstackedbar\n");
	return;
    }
    for (i = 0; i < g[gno].maxplot; i++) {
	if (isactive(gno, i)) {
	    x = getx(gno, i);
	    y = gety(gno, i);
	    cc = g[gno].p[i].color;
	    cy = g[gno].p[i].fillcolor;
	    py = g[gno].p[i].fillpattern;
	    ly = g[gno].p[i].lines;
	    wy = g[gno].p[i].linew;
	    c = setcolor(cy);
	    p = setpattern(py);
	    l = setlinestyle(ly);
	    w = setlinewidth(wy);
	    for (j = 0; j < maxn; j++) {
		if (j < getsetlength(gno, i)) {
		    if (g[gno].p[i].fill) {
			c = setcolor(cy);
			tmpy[0] = y[j] - bsize * 0.5;
			tmpx[0] = sum[j];
			tmpy[1] = y[j] - bsize * 0.5;
			tmpx[1] = sum[j] + x[j];
			tmpy[2] = y[j] + bsize * 0.5;
			tmpx[2] = sum[j] + x[j];
			tmpy[3] = y[j] + bsize * 0.5;
			tmpx[3] = sum[j];
			if (tmpy[0] > g[gno].w.yg2) {
			    continue;
			}
			if (tmpy[2] < g[gno].w.yg1) {
			    continue;
			}
			for (k = 0; k < 4; k++) {
			    if (tmpy[k] < g[gno].w.yg1)
				tmpy[k] = g[gno].w.yg1;
			    else if (tmpy[k] > g[gno].w.yg2)
				tmpy[k] = g[gno].w.yg2;
			    if (tmpx[k] < g[gno].w.xg1)
				tmpx[k] = g[gno].w.xg1;
			    else if (tmpx[k] > g[gno].w.xg2)
				tmpx[k] = g[gno].w.xg2;
			}
			if (g[gno].p[i].fillusing == PATTERN) {
			    fillrectpat(tmpx[0], tmpy[0], tmpx[2], tmpy[2]);
			} else if (g[gno].p[i].fillusing == COLOR) {
			    fillrectcolor(tmpx[0], tmpy[0], tmpx[2], tmpy[2]);
			}
		    }
		    if (ly && wy) {
			tmpy[0] = y[j] - bsize * 0.5;
			tmpx[0] = sum[j];
			tmpy[1] = y[j] - bsize * 0.5;
			tmpx[1] = sum[j] + x[j];
			tmpy[2] = y[j] + bsize * 0.5;
			tmpx[2] = sum[j] + x[j];
			tmpy[3] = y[j] + bsize * 0.5;
			tmpx[3] = sum[j];
			setcolor(cc);
			my_move2(tmpx[0], tmpy[0]);
			for (k = 0; k < 4; k++) {
			    my_draw2(tmpx[(k + 1) % 4], tmpy[(k + 1) % 4]);
			}
		    }
		    sum[j] += x[j];
		}
	    }
	}
    }
    free(sum);
}

/*
 * draw a set with error bars
 */
void drawseterrbar(int gno, int setno, double offsx, double offsy)
{
    int i, n = getsetlength(gno, setno);
    double *x = getx(gno, setno), *y = gety(gno, setno);
    double *dx, *dy;
    double ebarlen = g[gno].p[setno].errbarper;
    int etype = g[gno].p[setno].errbarxy;
    int c, w, l;
    int cy = g[gno].p[setno].color;
    int wy = g[gno].p[setno].linew;
    int ly = g[gno].p[setno].lines;

    c = setcolor(cy);
    l = setlinestyle(ly);
    w = setlinewidth(wy);

    switch (dataset_type(gno, setno)) {
    case XYDX:
    case XYDY:
	dx = getcol(gno, setno, 2);
	dy = getcol(gno, setno, 2);
	break;
    case XYDXDX:
    case XYDYDY:
    case XYDXDY:
	dx = getcol(gno, setno, 2);
	dy = getcol(gno, setno, 3);
	break;
    }

/*
 * draw the riser
 */
    if (g[gno].p[setno].errbar_riser == ON) {
	setlinestyle(g[gno].p[setno].errbar_riser_lines);
	setlinewidth(g[gno].p[setno].errbar_riser_linew);
	for (i = 0; i < n; i++) {
	    switch (dataset_type(gno, setno)) {
	    case XYDY:
	    case XYDYDY:
		switch (etype) {
		case BOTH:
		    my_move2(x[i] - offsx, y[i] - dy[i]);
		    my_draw2(x[i] - offsx, y[i] + dx[i]);
		    break;
		case TOP:
		    my_move2(x[i] - offsx, y[i]);
		    my_draw2(x[i] - offsx, y[i] + dx[i]);
		    break;
		case BOTTOM:
		    my_move2(x[i] - offsx, y[i] - dy[i]);
		    my_draw2(x[i] - offsx, y[i]);
		    break;
		}
		break;
	    case XYDX:
	    case XYDXDX:
		switch (etype) {
		case BOTH:
		    my_move2(x[i] - dy[i], y[i] - offsy);
		    my_draw2(x[i] + dx[i], y[i] - offsy);
		    break;
		case LEFT:
		    my_move2(x[i] - dy[i], y[i] - offsy);
		    my_draw2(x[i], y[i] - offsy);
		    break;
		case RIGHT:
		    my_move2(x[i], y[i] - offsy);
		    my_draw2(x[i] + dx[i], y[i] - offsy);
		    break;
		}
		break;
	    case XYDXDY:
		switch (etype) {
		case BOTH:
		    my_move2(x[i] - dx[i], y[i] - offsy);
		    my_draw2(x[i] + dx[i], y[i] - offsy);
		    my_move2(x[i] - offsx, y[i] - dy[i]);
		    my_draw2(x[i] - offsx, y[i] + dy[i]);
		    break;
		case LEFT:
		    my_move2(x[i] - dx[i], y[i] - offsy);
		    my_draw2(x[i], y[i] - offsy);
		    my_move2(x[i] - offsx, y[i] - dy[i]);
		    my_draw2(x[i] - offsx, y[i]);
		    break;
		case RIGHT:
		    my_move2(x[i] + dx[i], y[i] - offsy);
		    my_draw2(x[i], y[i] - offsy);
		    my_move2(x[i] - offsx, y[i] + dy[i]);
		    my_draw2(x[i] - offsx, y[i]);
		    break;
		}
		break;
	    }
	}
    }
/*
 * draw the bar
 */
    setlinestyle(g[gno].p[setno].errbar_lines);
    setlinewidth(g[gno].p[setno].errbar_linew);
    for (i = 0; i < n; i++) {
	switch (dataset_type(gno, setno)) {
	case XYDY:
	case XYDYDY:
	    switch (etype) {
	    case BOTH:
		errorbar(x[i] - offsx, y[i] - dy[i], ebarlen, 1);
		errorbar(x[i] - offsx, y[i] + dx[i], ebarlen, 1);
		break;
	    case TOP:
		errorbar(x[i] - offsx, y[i] + dx[i], ebarlen, 1);
		break;
	    case BOTTOM:
		errorbar(x[i] - offsx, y[i] - dy[i], ebarlen, 1);
		break;
	    }
	    break;
	case XYDX:
	case XYDXDX:
	    switch (etype) {
	    case BOTH:
		errorbar(x[i] - dy[i], y[i] - offsy, ebarlen, 0);
		errorbar(x[i] + dx[i], y[i] - offsy, ebarlen, 0);
		break;
	    case LEFT:
		errorbar(x[i] - dy[i], y[i] - offsy, ebarlen, 0);
		break;
	    case RIGHT:
		errorbar(x[i] + dx[i], y[i] - offsy, ebarlen, 0);
		break;
	    }
	    break;
	case XYDXDY:
	    switch (etype) {
	    case BOTH:
		errorbar(x[i] + dx[i], y[i] - offsy, ebarlen, 0);
		errorbar(x[i] - dx[i], y[i] - offsy, ebarlen, 0);
		errorbar(x[i] - offsx, y[i] - dy[i], ebarlen, 1);
		errorbar(x[i] - offsx, y[i] + dy[i], ebarlen, 1);
		break;
	    case LEFT:
		errorbar(x[i] - dx[i], y[i] - offsy, ebarlen, 0);
		errorbar(x[i] - offsx, y[i] - dy[i], ebarlen, 1);
		break;
	    case RIGHT:
		errorbar(x[i] + dx[i], y[i] - offsy, ebarlen, 0);
		errorbar(x[i] - offsx, y[i] + dy[i], ebarlen, 1);
		break;
	    }
	    break;
	}
    }
    setlinewidth(w);
    setlinestyle(l);
    setcolor(c);
}

void set_timestamp(void)
{
    struct tm tm;
    long time_value;
    char *str;

    (void) time(&time_value);
    tm = *localtime(&time_value);
    str = asctime(&tm);
    set_plotstr_string(&timestamp, str);
    timestamp.s[strlen(timestamp.s) - 1] = 0;
}

/*
 * draw each point in a set as a boxplot
 *
 * e[1] is the median,
 * e[2] is the lower inner value
 * e[3] is the upper inner value
 * e[4] is the lower whisker
 * e[5] is the upper whisker
 */
void drawsetboxplot(int gno, int setno)
{
    int i, n = getsetlength(gno, setno);
    double *x, *med;
    double *il, *iu;
    double *ol, *ou;
    double ebarlen = g[gno].p[setno].errbarper;
    int c, w, l;
    int cy = g[gno].p[setno].color;
    int wy = g[gno].p[setno].linew;
    int ly = g[gno].p[setno].lines;

    c = setcolor(cy);
    l = setlinestyle(ly);
    w = setlinewidth(wy);

    x = getcol(gno, setno, 0);
    med = getcol(gno, setno, 1);
    il = getcol(gno, setno, 2);
    iu = getcol(gno, setno, 3);
    ol = getcol(gno, setno, 4);
    ou = getcol(gno, setno, 5);
    for (i = 0; i < n; i++) {
	boxplotsym(x[i], med[i], il[i], iu[i], ol[i], ou[i], ebarlen);
    }
    setlinewidth(w);
    setlinestyle(l);
    setcolor(c);
}

void drawflow(int gno, int setno, velocityp vp)
{
    int i, j, c, l, w, cy, ly = vp.lines, wy = vp.linew;
    double *x = getx(gno, setno), *y = gety(gno, setno);
    double *u, *v;
    char s[256];

    cy = getsetcolor(gno, setno);
    u = getcol(gno, setno, 2);
    v = getcol(gno, setno, 3);
    if (u == NULL || v == NULL) {
	return;
    }
    c = setcolor(cy);
    l = setlinestyle(ly);
    w = setlinewidth(wy);
    for (i = 0; i < getsetlength(gno, setno); i++) {
	if (symok(x[i], y[i])) {
	    velplt(x[i], y[i], u[i], v[i], vp.vscale, vp.arrowtype);
	}
    }
    setlinestyle(l);
    setlinewidth(w);
    setcolor(c);
}

/*
 * draw a standard set with symbols and lines in a polar plot
 */
void drawsetxypolar(plotarr p, int i)
{
    int j, c, l, w, cy = p.color, sy = p.sym, ly = p.lines, wy = p.linew;
    double xbar, sd, *x = p.ex[0], *y = p.ex[1];
    char s[256];

    c = setcolor(cy);
    w = setlinewidth(wy);
    if (wy && ly) {
	l = setlinestyle(ly);
	setlinewidth(wy);
	drawpolarpoly(x, y, p.len);
	setlinestyle(l);
    }
    if (sy) {
	switch (sy) {
	case SYM_SEG:		/* draw segments */
	    drawpolyseg(x, y, p.len);
	    break;
	case SYM_CHAR:		/* draw character */
	    setfont(p.font);
	    if (p.symchar > ' ') {
		int skip = p.symskip + 1;

		s[0] = p.symchar;
		s[1] = 0;
		for (j = 0; j < p.len; j += skip) {
		    if (symok(x[j], y[j])) {
			writestr(x[j], y[j], 0, 2, s);
		    }
		}
	    }
	    break;
	case SYM_HILOX:	/* draw hilo along X */
	    if (p.ex[2] != NULL) {
		for (j = 0; j < p.len; j++) {
		    my_move2(x[j], y[j]);
		    my_draw2(p.ex[2][j], y[j]);
		}
	    }
	    break;
	case SYM_HILOY:	/* draw hilo along Y */
	    if (p.ex[2] != NULL) {
		for (j = 0; j < p.len; j++) {
		    my_move2(x[j], y[j]);
		    my_draw2(x[j], p.ex[2][j]);
		}
	    }
	    break;
	case SYM_OPENCLOSEX:	/* draw open/close along X */
	    if (p.ex[2] != NULL) {
		for (j = 0; j < p.len; j++) {
		    openclose(y[j], x[j], p.ex[2][j], 1.0, 0);
		}
	    }
	    break;
	case SYM_OPENCLOSEY:	/* draw open/close along Y */
	    if (p.ex[2] != NULL) {
		for (j = 0; j < p.len; j++) {
		    openclose(x[j], y[j], p.ex[2][j], 1.0, 1);
		}
	    }
	    break;
	case SYM_RANGE:	/* draw bounding box */
	    rect(p.xmin, p.ymin, p.xmax, p.ymax);
	    stasum(y, p.len, &xbar, &sd, 0);
	    my_move2(p.xmin, xbar);
	    my_draw2(p.xmax, xbar);
	    stasum(x, p.len, &xbar, &sd, 0);
	    my_move2(xbar, p.ymin);
	    my_draw2(xbar, p.ymax);
	    break;
	case SYM_TAG_FIRST:	/* tag the first point in a set */
	    if (symok(x[0], y[0])) {
		sprintf(s, "S%1d:1", i);
		sd = setcharsize(0.8);
		writestr(x[0], y[0], 0, 2, s);
		(void) setcharsize(sd);
	    }
	    break;
	case SYM_TAG_LAST:	/* tag the last point in a set */
	    if (symok(x[p.len - 1], y[p.len - 1])) {
		sprintf(s, "S%1d:%1d", i, p.len);
		sd = setcharsize(0.8);
		writestr(x[p.len - 1], y[p.len - 1], 0, 2, s);
		(void) setcharsize(sd);
	    }
	    break;
	case SYM_TAG_CENTER:	/* tag the middle point in a set */
	    if (symok(x[p.len / 2], y[p.len / 2])) {
		sprintf(s, "S%1d:%1d", i, p.len / 2);
		sd = setcharsize(0.8);
		writestr(x[p.len / 2], y[p.len / 2], 0, 2, s);
		(void) setcharsize(sd);
	    }
	    break;
	case SYM_STRING:	/* string at plot */
	    /* drawpolystring(x, y, p.len, sy, 0); */
	    break;
	case SYM_SETNO_LOC:	/* set number and location */
	case SYM_SETNO:	/* set number */
	    for (j = 0; j < p.len; j++) {
		if (symok(x[j], y[j])) {
		    if (sy == SYM_SETNO) {
			sprintf(s, "S%d", i);
		    } else {
			sprintf(s, "S%1d:%1d)", i, j + 1);
		    }
		    writestr(x[j], y[j], 0, 0, s);
		}
	    }
	    break;
	case SYM_AVGY:		/* average Y */
	    break;
	case SYM_AVGSTDY1:
	    break;
	case SYM_AVGSTDY2:
	    break;
	case SYM_AVGSTDY3:
	    break;
	case SYM_MEDIANY:
	    break;
	case SYM_GEOMMEANY:
	    break;
	case SYM_HARMMEANY:
	    break;
	default:
	    drawpolysym(x, y, p.len, sy, p.symskip, p.symfill, p.symsize);
	    break;
	}
    }
    setlinewidth(w);
    setcolor(c);
}

void drawisopat(double x1, double y1, double x2, double y2, double x3, double y3, double c1, double c2, double c3, int imin, int imax, Isolparms ip);

void drawfegrid(int gno, int setno)
{
    plotarr p = g[gno].p[setno];
    int i, j, c, l, w, cy = p.color, sy = p.sym, ly = p.lines, wy = p.linew;
    double xbar, sd, *x = p.ex[0], *y = p.ex[1], *z = p.ex[2];
    char s[256];
    int save = 0, n1, n2, n3, n4;
    extern int *colors;

    getlineprops(&c, &l, &w);
    if (wy && ly) {
	if (cy != c) {
	    setcolor(cy);
	}
	if (wy != w) {
	    setlinewidth(wy);
	}
	if (ly != l) {
	    setlinestyle(ly);
	}
	for (i = 0; i < p.nelem; i++) {
/*
            drawisopat(
	    x[p.elements[0][i]], y[p.elements[0][i]],
	    x[p.elements[1][i]], y[p.elements[1][i]],
	    x[p.elements[2][i]], y[p.elements[2][i]],
	    z[p.elements[0][i]], z[p.elements[1][i]], z[p.elements[2][i]],
		0, g[gno].isol.nisol, g[gno].isol);

            drawiso(
	    x[p.elements[0][i]], y[p.elements[0][i]],
	    x[p.elements[1][i]], y[p.elements[1][i]],
	    x[p.elements[2][i]], y[p.elements[2][i]],
	    z[p.elements[0][i]], z[p.elements[1][i]], z[p.elements[2][i]],
		0, g[gno].isol.nisol - 1, g[gno].isol);
*/

	    my_move2(x[p.elements[0][i]], y[p.elements[0][i]]);
	    for (j = 1; j < p.nnodes[i]; j++) {
		my_draw2(x[p.elements[j][i]], y[p.elements[j][i]]);
	    }
	    my_draw2(x[p.elements[0][i]], y[p.elements[0][i]]);
	}
    }
    setcolor(1);
    setlinewidth(1);
    g[gno].isol.p.color = 1;
    if (g[gno].isol.lactive == ON) {
	doisollegend(g[gno].isol, colors, maxcolors);
    }
}

int inrange(double c, double c1, double c2)
{   
    return ((c >= c1 && c <= c2) || (c >= c2 && c <= c1));
}   

void drawisopat(double x1, double y1, double x2, double y2, double x3, double y3, double c1, double c2, double c3, int imin, int imax, Isolparms ip)
{
    int i, j, k, l = 0, cnt = 0;
    int n1, n2, n3, nmin;
    double cc;
    double px[50], py[50];
    double x[3], y[3], c[3];
    char buf[80];
    double cc1, cc2, cmin, cmax;
    x[0] = x1;
    y[0] = y1;
    x[1] = x2;
    y[1] = y2;
    x[2] = x3;
    y[2] = y3;
    c[0] = c1;
    c[1] = c2;
    c[2] = c3;
    for (k = imin; k < imax; k++) {
	cc1 = ip.cis[k];
	cc2 = ip.cis[k + 1];
	l = 0;
	for (j = 0; j < 3; j++) {
	    if (inrange(c[j], cc1, cc2)) {
		px[l] = x[j];
		py[l++] = y[j];
	    }
	    if (c[j] < c[(j + 1) % 3]) {
		if (inrange(cc1, c[j], c[(j + 1) % 3])) {
		    if (c[(j + 1) % 3] != c[j]) {
			px[l] = (cc1 - c[j]) * (x[(j + 1) % 3] - x[j]) / (c[(j + 1) % 3] - c[j]) + x[j];
			py[l++] = (cc1 - c[j]) * (y[(j + 1) % 3] - y[j]) / (c[(j + 1) % 3] - c[j]) + y[j];
		    }
		}
		if (inrange(cc2, c[j], c[(j + 1) % 3])) {
		    if (c[(j + 1) % 3] != c[j]) {
			px[l] = (cc2 - c[j]) * (x[(j + 1) % 3] - x[j]) / (c[(j + 1) % 3] - c[j]) + x[j];
			py[l++] = (cc2 - c[j]) * (y[(j + 1) % 3] - y[j]) / (c[(j + 1) % 3] - c[j]) + y[j];
		    }
		}
	    } else if (c[j] > c[(j + 1) % 3]) {
		if (inrange(cc2, c[j], c[(j + 1) % 3])) {
		    if (c[(j + 1) % 3] != c[j]) {
			px[l] = (cc2 - c[j]) * (x[(j + 1) % 3] - x[j]) / (c[(j + 1) % 3] - c[j]) + x[j];
			py[l++] = (cc2 - c[j]) * (y[(j + 1) % 3] - y[j]) / (c[(j + 1) % 3] - c[j]) + y[j];
		    }
		}
		if (inrange(cc1, c[j], c[(j + 1) % 3])) {
		    if (c[(j + 1) % 3] != c[j]) {
			px[l] = (cc1 - c[j]) * (x[(j + 1) % 3] - x[j]) / (c[(j + 1) % 3] - c[j]) + x[j];
			py[l++] = (cc1 - c[j]) * (y[(j + 1) % 3] - y[j]) / (c[(j + 1) % 3] - c[j]) + y[j];
		    }
		}
	    }
	}
	if (l >= 2 && k < ip.nisol - 1) {
	    setcolor(k + 6);
	    fillcolor(l, px, py);
	}
    }
}
