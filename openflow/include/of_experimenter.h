/*A.3.5.15 Experimenter Multipart
Experimenter-specic multipart messages are requested with the OFPMP_EXPERIMENTER multipart type.
The rst bytes of the request and reply bodies are the following structure:*/
/* Body for ofp_multipart_request/reply of type OFPMP_EXPERIMENTER. */
struct ofp_experimenter_multipart_header {
    uint32_t experimenter; /* Experimenter ID which takes the same form
    as in struct ofp_experimenter_header. */
    uint32_t exp_type; /* Experimenter defined. */
    /* Experimenter-defined arbitrary additional data. */
};
OFP_ASSERT(sizeof(struct ofp_experimenter_multipart_header) == 8);

/*A.5.4 Experimenter
The Experimenter message is dened as follows:*/
/* Experimenter extension. */
struct ofp_experimenter_header {
    struct ofp_header header; /* Type OFPT_EXPERIMENTER. */
    uint32_t experimenter; /* Experimenter ID:
    * - MSB 0: low-order bytes are IEEE OUI.
    * - MSB != 0: defined by ONF. */
    uint32_t exp_type; /* Experimenter defined. */
    /* Experimenter-defined arbitrary additional data. */
};
OFP_ASSERT(sizeof(struct ofp_experimenter_header) == 16);

/*The experimenter eld is a 32-bit value that uniquely identies the experimenter. If the most signicant
byte is zero, the next three bytes are the experimenter's IEEE OUI. If the most signicant byte is not
zero, it is a value allocated by the Open Networking Foundation. If experimenter does not have (or wish
to use) their OUI, they should contact the Open Networking Foundation to obtain a unique experimenter
ID.
The rest of the body is uninterpreted by standard OpenFlow processing and is arbitrarily dened by
the corresponding experimenter.
If a switch does not understand a experimenter extension, it must send an OFPT_ERROR message with a
OFPBRC_BAD_EXPERIMENTER error code and OFPET_BAD_REQUEST error type.*/

