
/*The experimenter eld is the Experimenter ID, which takes the same form as in struct
ofp_experimenter (see A.5.4).*/
/*A.5 Symmetric Messages*/
/*A.5.1 Hello
The OFPT_HELLO message consists of an OpenFlow header plus a set of variable size hello elements.*/
/* OFPT_HELLO. This message includes zero or more hello elements having
* variable size. Unknown elements types must be ignored/skipped, to allow
* for future extensions. */
struct ofp_hello {
    struct ofp_header header;
    /* Hello element list */
    struct ofp_hello_elem_header elements[0];
};
OFP_ASSERT(sizeof(struct ofp_hello) == 8);

/*The version eld part of the header eld (see A.1) must be set to the highest OpenFlow protocol
version supported by the sender (see 6.3.1).
The elements eld is a set of hello elements, containing optional data to inform the initial handshake
of the connection. Implementations must ignore (skip) all elements of a Hello message that they do not
support. The list of hello elements types that are currently dened are:*/
/* Hello elements types.
*/
enum ofp_hello_elem_type {
    OFPHET_VERSIONBITMAP = 1, /* Bitmap of version supported. */
};

/*An element denition contains the element type, length, and any associated data:*/
/* Common header for all Hello Elements */
struct ofp_hello_elem_header {
    uint16_t type; /* One of OFPHET_*. */
    uint16_t length; /* Length in bytes of this element. */
};
OFP_ASSERT(sizeof(struct ofp_hello_elem_header) == 4);

/*The OFPHET_VERSIONBITMAP element use the following structure and elds:*/
/* Version bitmap Hello Element */
struct ofp_hello_elem_versionbitmap {
    uint16_t type; /* OFPHET_VERSIONBITMAP. */
    uint16_t length; /* Length in bytes of this element. */
    /* Followed by:
    * - Exactly (length - 4) bytes containing the bitmaps, then
    * - Exactly (length + 7)/8*8 - (length) (between 0 and 7)
    * bytes of all-zero bytes */
    uint32_t bitmaps[0]; /* List of bitmaps - supported versions */
};
OFP_ASSERT(sizeof(struct ofp_hello_elem_versionbitmap) == 4);

/*The bitmaps eld indicates the set of versions of the OpenFlow switch protocol a device supports,
and may be used during version negotiation (see 6.3.1). The bits of the set of bitmaps are indexed
by the ofp version number of the protocol ; if the bit identied by the number of left bitshift equal
to a ofp version number is set, this OpenFlow version is supported. The number of bitmaps included
in the eld depend on the highest version number supported : ofp versions 0 to 31 are encoded in
the rst bitmap, ofp versions 32 to 63 are encoded in the second bitmap and so on. For example, if
a switch supports only version 1.0 (ofp version=0x01) and version 1.3 (ofp version=0x04), the rst
bitmap would be set to 0x00000012.
A.5.2 Echo Request
An Echo Request message consists of an OpenFlow header plus an arbitrary-length data eld. The
data eld might be a message timestamp to check latency, various lengths to measure bandwidth, or
zero-size to verify liveness between the switch and controller.
A.5.3 Echo Reply
An Echo Reply message consists of an OpenFlow header plus the unmodied data eld of an echo
request message.
In an OpenFlow protocol implementation divided into multiple layers, the echo request/reply logic
should be implemented in the "deepest" practical layer. For example, in the OpenFlow reference
implementation that includes a userspace process that relays to a kernel module, echo request/reply is
implemented in the kernel module. Receiving a correctly formatted echo reply then shows a greater
likelihood of correct end-to-end functionality than if the echo request/reply were implemented in the
userspace process, as well as providing more accurate end-to-end latency timing.*/
