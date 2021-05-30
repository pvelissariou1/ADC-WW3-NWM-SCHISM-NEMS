/* $Id: printwin.c,v 1.1.1.1 2003/07/21 16:18:41 pturner Exp $
 *
 * Printer initialization
 */

#include <stdio.h>

#include <Xm/Xm.h>
#include <Xm/BulletinB.h>
#include <Xm/DialogS.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>

#include "globals.h"
#include "motifinc.h"

extern int epsflag;		/* declared in ps.c */

static Widget *printto_item;	/* for printer select popup */
static Widget printstring_item;
static Widget psetup_frame;
static Widget psetup_panel;
static Widget *devices_item;
static Widget eps_item;

static void do_pr_toggle(Widget w, XtPointer client_data, XtPointer call_data);
static void update_printer_setup(void);
static void set_printer_proc(Widget w, XtPointer client_data, XtPointer call_data);
static void do_print(Widget w, XtPointer client_data, XtPointer call_data);

/*
 * set the current print options
 */
void set_printer(int device, char *prstr)
{
    if (device == FILEP) {
	if (prstr != NULL) {
	    strcpy(printstr, prstr);
	}
	ptofile = TRUE;
    } else {
	switch (device) {
	case GR_PS_L:
	case GR_PS_P:
	    if (prstr != NULL) {
		strcpy(ps_prstr, prstr);
	    }
	    curprint = ps_prstr;
	    break;
	case GR_MIF_L:
	case GR_MIF_P:
	    if (prstr != NULL) {
		strcpy(mif_prstr, prstr);
	    }
	    curprint = mif_prstr;
	    break;
	case GR_GIFP:
	case GR_GIFL:
	    if (prstr != NULL) {
		strcpy(gd_prstr, prstr);
	    }
	    curprint = gd_prstr;
	    break;
	default:
	    sprintf(buf, "Unknown printer device %d, printer unchanged", device);
	    errwin(buf);
	    return;
	    break;
	}
	hdevice = device;
	ptofile = FALSE;
    }
    if (psetup_frame) {
	update_printer_setup();
    }
}

/*
 * set the print options
 */
void do_prstr_toggle(Widget w, XtPointer client_data, XtPointer call_data)
{
    int value = (int) client_data;
    set_printer(value + 1, NULL);
    if ((int) GetChoice(printto_item) == 0) {
	xv_setstr(printstring_item, curprint);
    }
}

static void do_pr_toggle(Widget w, XtPointer client_data, XtPointer call_data)
{
    int value = (int) client_data;
    if (value) {
	xv_setstr(printstring_item, printstr);
    } else {
	xv_setstr(printstring_item, curprint);
    }
}

void create_printer_setup(Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget wbut, rc;
    int i, x, y;
    set_wait_cursor();
    if (psetup_frame == NULL) {
	Widget buts[3];
	char *label1[3];
	label1[0] = "Accept (no print)";
	label1[1] = "Print";
	label1[2] = "Cancel";
	XmGetPos(app_shell, 0, &x, &y);
	psetup_frame = XmCreateDialogShell(app_shell, "Printer setup", NULL, 0);
	handle_close(psetup_frame);
	XtVaSetValues(psetup_frame, XmNx, x, XmNy, y, NULL);
	psetup_panel = XmCreateRowColumn(psetup_frame, "psetup_rc", NULL, 0);

	rc = XtVaCreateWidget("rc", xmRowColumnWidgetClass, psetup_panel,
			      XmNpacking, XmPACK_COLUMN,
			      XmNnumColumns, 4,
			      XmNorientation, XmHORIZONTAL,
			      XmNisAligned, True,
			      XmNadjustLast, False,
			      XmNentryAlignment, XmALIGNMENT_END,
			      NULL);

	XtVaCreateManagedWidget("Device: ", xmLabelWidgetClass, rc, NULL);
	devices_item = CreatePanelChoice(rc, " ",
					 7,
					 "PostScript landscape",
					 "PostScript portrait",
					 "FrameMaker landscape",
					 "FrameMaker portrait",
					 "GIF landscape",
					 "GIF portrait",
					 0, 0);
	for (i = 0; i < 6; i++) {
	    XtAddCallback(devices_item[2 + i], XmNactivateCallback,
			  (XtCallbackProc) do_prstr_toggle, (XtPointer) i);
	}

	XtVaCreateManagedWidget("Print to: ", xmLabelWidgetClass, rc, NULL);
	printto_item = CreatePanelChoice(rc, " ",
					 3,
					 "Printer",
					 "File", 0, 0);
	for (i = 0; i < 2; i++) {
	    XtAddCallback(printto_item[2 + i], XmNactivateCallback,
			  (XtCallbackProc) do_pr_toggle, (XtPointer) i);
	}

	printstring_item = CreateTextItem4(rc, 20, "Print control string:");

	XtVaCreateManagedWidget("Generate EPS ", xmLabelWidgetClass, rc, NULL);
	eps_item = XmCreateToggleButton(rc, " ", NULL, 0);
	XtManageChild(eps_item);
	XtManageChild(rc);

	XtVaCreateManagedWidget("sep", xmSeparatorWidgetClass, psetup_panel, NULL);

	CreateCommandButtons(psetup_panel, 3, buts, label1);
	XtAddCallback(buts[0], XmNactivateCallback,
		      (XtCallbackProc) set_printer_proc, (XtPointer) NULL);
	XtAddCallback(buts[1], XmNactivateCallback,
		      (XtCallbackProc) do_print, (XtPointer) NULL);
	XtAddCallback(buts[2], XmNactivateCallback,
		 (XtCallbackProc) destroy_dialog, (XtPointer) psetup_frame);
	XtManageChild(psetup_panel);
    }
    XtRaise(psetup_frame);
    update_printer_setup();
    unset_wait_cursor();
}

static void update_printer_setup(void)
{
    SetChoice(devices_item, hdevice - 1);
    SetChoice(printto_item, ptofile);
    XmToggleButtonSetState(eps_item, epsflag, False);
    if (hdevice != 1 && hdevice != 2) {
	XtSetSensitive(eps_item, False);
    }
    else {
	XtSetSensitive(eps_item, True);
    }
    if (ptofile) {
	xv_setstr(printstring_item, printstr);
    } else {
	xv_setstr(printstring_item, curprint);
    }
}

static void set_printer_proc(Widget w, XtPointer client_data, XtPointer call_data)
{
    char tmpstr[128];
    hdevice = (int) GetChoice(devices_item) + 1;
    ptofile = (int) GetChoice(printto_item);
    epsflag = XmToggleButtonGetState(eps_item);
    strcpy(tmpstr, (char *) xv_getstr(printstring_item));
    if (ptofile) {
	strcpy(printstr, tmpstr);
    } else {
	strcpy(curprint, tmpstr);
    }
    XtUnmanageChild(psetup_frame);
}

/*
 * Print button
 */
static void do_print(Widget w, XtPointer client_data, XtPointer call_data)
{
    set_wait_cursor();
    set_printer_proc(NULL, NULL, NULL);
    do_hardcopy();
    unset_wait_cursor();
}
