
/*A.3.5.1 Description
Information about the switch manufacturer, hardware revision, software revision, serial number, and a
description eld is available from the OFPMP_DESC multipart request type:*/
/* Body of reply to OFPMP_DESC request. Each entry is a NULL-terminated
* ASCII string. */
struct ofp_desc {
    char mfr_desc[DESC_STR_LEN]; /* Manufacturer description. */
    char hw_desc[DESC_STR_LEN]; /* Hardware description. */
    char sw_desc[DESC_STR_LEN]; /* Software description. */
    char serial_num[SERIAL_NUM_LEN]; /* Serial number. */
    char dp_desc[DESC_STR_LEN]; /* Human readable description of datapath. */
};
OFP_ASSERT(sizeof(struct ofp_desc) == 1056);
/*Each entry is ASCII formatted and padded on the right with null bytes (n0). DESC_STR_LEN is 256 and
SERIAL_NUM_LEN is 32. The dp_desc eld is a free-form string to describe the datapath for debugging
purposes, e.g., \switch3 in room 3120". As such, it is not guaranteed to be unique and should not be
used as the primary identier for the datapath|use the datapath_id eld from the switch features
instead (see A.3.1).*/

