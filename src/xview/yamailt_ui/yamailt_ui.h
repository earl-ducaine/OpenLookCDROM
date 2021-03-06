#ifndef	yamailt_HEADER
#define	yamailt_HEADER

/*
 * yamailt_ui.h - User interface object and function declarations.
 * This file was generated by `gxv' from `yamailt.G'.
 * DO NOT EDIT BY HAND.
 */

extern Attr_attribute	INSTANCE;


typedef struct {
	Xv_opaque	baseWindow;
	Xv_opaque	controls1;
	Xv_opaque	fileButton;
	Xv_opaque	viewButton;
	Xv_opaque	editButton;
	Xv_opaque	composeButton;
	Xv_opaque	setting1;
	Xv_opaque	prevButton;
	Xv_opaque	nextButton;
	Xv_opaque	deleteButton;
	Xv_opaque	replyButton;
	Xv_opaque	button9;
	Xv_opaque	button10;
	Xv_opaque	button11;
	Xv_opaque	list1;
} yamailt_baseWindow_objects;

extern yamailt_baseWindow_objects	*yamailt_baseWindow_objects_initialize(yamailt_baseWindow_objects *, Xv_opaque);

extern Xv_opaque	yamailt_baseWindow_baseWindow_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_controls1_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_fileButton_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_viewButton_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_editButton_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_composeButton_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_setting1_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_prevButton_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_nextButton_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_deleteButton_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_replyButton_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_button9_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_button10_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_button11_create(yamailt_baseWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_baseWindow_list1_create(yamailt_baseWindow_objects *, Xv_opaque);

typedef struct {
	Xv_opaque	compWindow;
	Xv_opaque	controls2;
	Xv_opaque	button12;
	Xv_opaque	button13;
	Xv_opaque	button14;
	Xv_opaque	button15;
	Xv_opaque	button16;
	Xv_opaque	toTextfield;
	Xv_opaque	subjectTextfield;
	Xv_opaque	ccTextfield;
	Xv_opaque	group1;
	Xv_opaque	message1;
	Xv_opaque	button17;
	Xv_opaque	button18;
} yamailt_compWindow_objects;

extern yamailt_compWindow_objects	*yamailt_compWindow_objects_initialize(yamailt_compWindow_objects *, Xv_opaque);

extern Xv_opaque	yamailt_compWindow_compWindow_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_controls2_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_button12_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_button13_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_button14_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_button15_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_button16_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_toTextfield_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_subjectTextfield_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_ccTextfield_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_group1_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_message1_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_button17_create(yamailt_compWindow_objects *, Xv_opaque);
extern Xv_opaque	yamailt_compWindow_button18_create(yamailt_compWindow_objects *, Xv_opaque);

typedef struct {
	Xv_opaque	sendPopup;
	Xv_opaque	controls3;
	Xv_opaque	stdSetting;
	Xv_opaque	whichKeySetting;
	Xv_opaque	encSetting;
} yamailt_sendPopup_objects;

extern yamailt_sendPopup_objects	*yamailt_sendPopup_objects_initialize(yamailt_sendPopup_objects *, Xv_opaque);

extern Xv_opaque	yamailt_sendPopup_sendPopup_create(yamailt_sendPopup_objects *, Xv_opaque);
extern Xv_opaque	yamailt_sendPopup_controls3_create(yamailt_sendPopup_objects *, Xv_opaque);
extern Xv_opaque	yamailt_sendPopup_stdSetting_create(yamailt_sendPopup_objects *, Xv_opaque);
extern Xv_opaque	yamailt_sendPopup_whichKeySetting_create(yamailt_sendPopup_objects *, Xv_opaque);
extern Xv_opaque	yamailt_sendPopup_encSetting_create(yamailt_sendPopup_objects *, Xv_opaque);
#endif
