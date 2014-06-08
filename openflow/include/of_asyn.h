/*A.4 Asynchronous Messages
A.4.1 Packet-In Message
When packets are received by the datapath and sent to the controller, they use the OFPT_PACKET_IN
message:*/
/* Packet received on port (datapath -> controller). */
struct ofp_packet_in {
    struct ofp_header header;
    uint32_t buffer_id; /* ID assigned by datapath. */
    uint16_t total_len; /* Full length of frame. */
    uint8_t reason; /* Reason packet is being sent (one of OFPR_*) */
    uint8_t table_id; /* ID of the table that was looked up */
    uint64_t cookie; /* Cookie of the flow entry that was looked up. */
    struct ofp_match match; /* Packet metadata. Variable size. */
    /* Followed by:
    * - Exactly 2 all-zero padding bytes, then
    * - An Ethernet frame whose length is inferred from header.length.
    * The padding bytes preceding the Ethernet frame ensure that the IP
    * header (if any) following the Ethernet header is 32-bit aligned.
    */
    //uint8_t pad[2]; /* Align to 64 bit + 16 bit */
    //uint8_t data[0]; /* Ethernet frame */
};
OFP_ASSERT(sizeof(struct ofp_packet_in) == 32);
/*The buffer_id is an opaque value used by the datapath to identify a buered packet. When a packet
is buered, some number of bytes from the message will be included in the data portion of the message.
If the packet is sent because of a \send to controller" action, then max_len bytes from the
ofp_action_output of the 
ow setup request are sent. If the packet is sent for other reasons, such
as an invalid TTL, then at least miss_send_len bytes from the OFPT_SET_CONFIG message are sent.
The default miss_send_len is 128 bytes. If the packet is not buered - either because of no available
buers, or because of explicitly requested via OFPCML_NO_BUFFER - the entire packet is included in the
data portion, and the buffer_id is OFP_NO_BUFFER.
Switches that implement buering are expected to expose, through documentation, both the amount
of available buering, and the length of time before buers may be reused. A switch must gracefully
handle the case where a buered packet_in message yields no response from the controller. A switch
should prevent a buer from being reused until it has been handled by the controller, or some amount
of time (indicated in documentation) has passed.
The data eld contains the packet itself, or a fraction of the packet if the packet is buered. The packet
header re
ect any changes applied to the packet in previous processing.
The reason eld can be any of these values:*/
/* Why is this packet being sent to the controller? */
enum ofp_packet_in_reason {
    OFPR_NO_MATCH = 0, /* No matching flow (table-miss flow entry). */
    OFPR_ACTION = 1, /* Action explicitly output to controller. */
    OFPR_INVALID_TTL = 2, /* Packet has invalid TTL */
};
/*
OFPR_INVALID_TTL indicates that a packets with an invalid IP TTL or MPLS TTL was rejected by
the OpenFlow pipeline and passed to the controller. Checking for invalid TTL does not need to
be done for every packet, but it must be done at a minimum every time a OFPAT_DEC_MPLS_TTL or
OFPAT_DEC_NW_TTL action is applied to a packet.
The cookie eld contains the cookie of the 
ow entry that caused the packet to be sent to the controller.
This eld must be set to -1 (0x) if a cookie cannot be associated with a particular 
ow. For
example, if the packet-in was generated in a group bucket or from the action set.
The match eld re
ect the packet's headers and context when the event that triggers the packet-in message
occurred and contains a set of OXM TLVs. This context includes any changes applied to the packet
in previous processing, including actions already executed, if any, but not any changes in the action set.
The OXM TLVs must include context elds, that is, elds whose values cannot be determined from
the packet data. The standard context elds are OFPXMT_OFB_IN_PORT, OFPXMT_OFB_IN_PHY_PORT,
OFPXMT_OFB_METADATA and OFPXMT_OFB_TUNNEL_ID. Fields whose values are all-bits-zero should be
omitted. Optionally, the OXM TLVs may also include packet header elds that were previously extracted
from the packet, including any modications of those in the course of the processing.
When a packet is received directly on a physical port and not processed by a logical port,
OFPXMT_OFB_IN_PORT and OFPXMT_OFB_IN_PHY_PORT have the same value, the OpenFlow port no
of this physical port. OFPXMT_OFB_IN_PHY_PORT should be omitted if it has the same value as
OFPXMT_OFB_IN_PORT.
When a packet is received on a logical port by way of a physical port, OFPXMT_OFB_IN_PORT is the logical
port's port no and OFPXMT_OFB_IN_PHY_PORT is the physical port's port no. For example, consider a
packet received on a tunnel interface dened over a link aggregation group (LAG) with two physical port
members. If the tunnel interface is the logical port bound to OpenFlow, then OFPXMT_OFB_IN_PORT is
the tunnel port no and OFPXMT_OFB_IN_PHY_PORT is the physical port no member of the LAG on which
the tunnel is congured.
The port referenced by the OFPXMT_OFB_IN_PORT TLV must be the port used for matching 
ow entries
(see 5.3) and must be available to OpenFlow processing (i.e. OpenFlow can forward packet to this port,
depending on port 
ags). OFPXMT_OFB_IN_PHY_PORT need not be available for matching or OpenFlow
processing.*/

/*A.4.2 Flow Removed Message
If the controller has requested to be notied when 
ow entries time out or are deleted from tables, the
datapath does this with the OFPT_FLOW_REMOVED message:*/
/* Flow removed (datapath -> controller). */
struct ofp_flow_removed {
    struct ofp_header header;
    uint64_t cookie; /* Opaque controller-issued identifier. */
    uint16_t priority; /* Priority level of flow entry. */
    uint8_t reason; /* One of OFPRR_*. */
    uint8_t table_id; /* ID of the table */
    uint32_t duration_sec; /* Time flow was alive in seconds. */
    uint32_t duration_nsec; /* Time flow was alive in nanoseconds beyond
    duration_sec. */
    uint16_t idle_timeout; /* Idle timeout from original flow mod. */
    uint16_t hard_timeout; /* Hard timeout from original flow mod. */
    uint64_t packet_count;
    uint64_t byte_count;
    struct ofp_match match; /* Description of fields. Variable size. */
};
OFP_ASSERT(sizeof(struct ofp_flow_removed) == 56);
/*The match, cookie, and priority elds are the same as those used in the 
ow mod request.
The reason eld is one of the following:*/
/* Why was this flow removed? */
enum ofp_flow_removed_reason {
    OFPRR_IDLE_TIMEOUT = 0, /* Flow idle time exceeded idle_timeout. */
    OFPRR_HARD_TIMEOUT = 1, /* Time exceeded hard_timeout. */
    OFPRR_DELETE = 2, /* Evicted by a DELETE flow mod. */
    OFPRR_GROUP_DELETE = 3, /* Group was removed. */
};

/*The duration_sec and duration_nsec elds are described in Section A.3.5.2.
The idle_timeout and hard_timeout elds are directly copied from the 
ow mod that created this
entry.
With the above three elds, one can nd both the amount of time the 
ow entry was active, as well as
the amount of time the 
ow entry received trac.
The packet_count and byte_count indicate the number of packets and bytes that were associated with
this 
ow entry, respectively. Those counters should behave like other statistics counters (see A.3.5) ;
they are unsigned and should be set to the maximum eld value if not available.*/

/*A.4.3 Port Status Message
As ports are added, modied, and removed from the datapath, the controller needs to be informed with
the OFPT_PORT_STATUS message:*/
/* A physical port has changed in the datapath */
struct ofp_port_status {
    struct ofp_header header;
    uint8_t reason; /* One of OFPPR_*. */
    uint8_t pad[7]; /* Align to 64-bits. */
    struct ofp_port desc;
};
OFP_ASSERT(sizeof(struct ofp_port_status) == 80);

/*The reason can be one of the following values:*/
/* What changed about the physical port */
enum ofp_port_reason {
    OFPPR_ADD = 0, /* The port was added. */
    OFPPR_DELETE = 1, /* The port was removed. */
    OFPPR_MODIFY = 2, /* Some attribute of the port has changed. */
};

/*A.4.4 Error Message
There are times that the switch needs to notify the controller of a problem. This is done with the
OFPT_ERROR_MSG message:*/
/* OFPT_ERROR: Error message (datapath -> controller). */
struct ofp_error_msg {
    struct ofp_header header;
    uint16_t type;
    uint16_t code;
    uint8_t data[0]; /* Variable-length data. Interpreted based
    on the type and code. No padding. */
};
OFP_ASSERT(sizeof(struct ofp_error_msg) == 12);

/*The type value indicates the high-level type of error. The code value is interpreted based on the type.
The data is variable length and interpreted based on the type and code. Unless specied otherwise, the
data eld contains at least 64 bytes of the failed request that caused the error message to be generated,
if the failed request is shorter than 64 bytes it should be the full request without any padding.
If the error message is in response to a specic message from the controller, e.g., OFPET_BAD_REQUEST,
OFPET_BAD_ACTION, OFPET_BAD_INSTRUCTION, OFPET_BAD_MATCH, or OFPET_FLOW_MOD_FAILED, then
the xid eld of the header must match that of the oending message.
Error codes ending in _EPERM correspond to a permissions error generated by, for example, an OpenFlow
hypervisor interposing between a controller and switch.
Currently dened error types are:*/
/* Values for 'type' in ofp_error_message. These values are immutable: they
* will not change in future versions of the protocol (although new values may
* be added). */
enum ofp_error_type {
    OFPET_HELLO_FAILED = 0, /* Hello protocol failed. */
    OFPET_BAD_REQUEST = 1, /* Request was not understood. */
    OFPET_BAD_ACTION = 2, /* Error in action description. */
    OFPET_BAD_INSTRUCTION = 3, /* Error in instruction list. */
    OFPET_BAD_MATCH = 4, /* Error in match. */
    OFPET_FLOW_MOD_FAILED = 5, /* Problem modifying flow entry. */
    OFPET_GROUP_MOD_FAILED = 6, /* Problem modifying group entry. */
    OFPET_PORT_MOD_FAILED = 7, /* Port mod request failed. */
    OFPET_TABLE_MOD_FAILED = 8, /* Table mod request failed. */
    OFPET_QUEUE_OP_FAILED = 9, /* Queue operation failed. */
    OFPET_SWITCH_CONFIG_FAILED = 10, /* Switch config request failed. */
    OFPET_ROLE_REQUEST_FAILED = 11, /* Controller Role request failed. */
    OFPET_METER_MOD_FAILED = 12, /* Error in meter. */
    OFPET_TABLE_FEATURES_FAILED = 13, /* Setting table features failed. */
    OFPET_EXPERIMENTER = 0xffff /* Experimenter error messages. */
};

/*For the OFPET_HELLO_FAILED error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_HELLO_FAILED. 'data' contains an
* ASCII text string that may give failure details. */
enum ofp_hello_failed_code {
    OFPHFC_INCOMPATIBLE = 0, /* No compatible version. */
    OFPHFC_EPERM = 1, /* Permissions error. */
};

/*The data eld contains an ASCII text string that adds detail on why the error occurred.
For the OFPET_BAD_REQUEST error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_BAD_REQUEST. 'data' contains at least
* the first 64 bytes of the failed request. */
enum ofp_bad_request_code {
    OFPBRC_BAD_VERSION = 0, /* ofp_header.version not supported. */
    OFPBRC_BAD_TYPE = 1, /* ofp_header.type not supported. */
    OFPBRC_BAD_MULTIPART = 2, /* ofp_multipart_request.type not supported. */
    OFPBRC_BAD_EXPERIMENTER = 3, /* Experimenter id not supported
    * (in ofp_experimenter_header or
    * ofp_multipart_request or
    * ofp_multipart_reply). */
    OFPBRC_BAD_EXP_TYPE = 4, /* Experimenter type not supported. */
    OFPBRC_EPERM = 5, /* Permissions error. */
    OFPBRC_BAD_LEN = 6, /* Wrong request length for type. */
    OFPBRC_BUFFER_EMPTY = 7, /* Specified buffer has already been used. */
    OFPBRC_BUFFER_UNKNOWN = 8, /* Specified buffer does not exist. */
    OFPBRC_BAD_TABLE_ID = 9, /* Specified table-id invalid or does not
    * exist. */
    OFPBRC_IS_SLAVE = 10, /* Denied because controller is slave. */
    OFPBRC_BAD_PORT = 11, /* Invalid port. */
    OFPBRC_BAD_PACKET = 12, /* Invalid packet in packet-out. */
    OFPBRC_MULTIPART_BUFFER_OVERFLOW = 13, /* ofp_multipart_request
    overflowed the assigned buffer. */
};

/*For the OFPET_BAD_ACTION error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_BAD_ACTION. 'data' contains at least
* the first 64 bytes of the failed request. */
enum ofp_bad_action_code {
    OFPBAC_BAD_TYPE = 0, /* Unknown action type. */
    OFPBAC_BAD_LEN = 1, /* Length problem in actions. */
    OFPBAC_BAD_EXPERIMENTER = 2, /* Unknown experimenter id specified. */
    OFPBAC_BAD_EXP_TYPE = 3, /* Unknown action for experimenter id. */
    OFPBAC_BAD_OUT_PORT = 4, /* Problem validating output port. */
    OFPBAC_BAD_ARGUMENT = 5, /* Bad action argument. */
    OFPBAC_EPERM = 6, /* Permissions error. */
    OFPBAC_TOO_MANY = 7, /* Can't handle this many actions. */
    OFPBAC_BAD_QUEUE = 8, /* Problem validating output queue. */
    OFPBAC_BAD_OUT_GROUP = 9, /* Invalid group id in forward action. */
    OFPBAC_MATCH_INCONSISTENT = 10, /* Action can't apply for this match,
    or Set-Field missing prerequisite. */
    OFPBAC_UNSUPPORTED_ORDER = 11, /* Action order is unsupported for the
    action list in an Apply-Actions instruction */
    OFPBAC_BAD_TAG = 12, /* Actions uses an unsupported
    tag/encap. */
    OFPBAC_BAD_SET_TYPE = 13, /* Unsupported type in SET_FIELD action. */
    OFPBAC_BAD_SET_LEN = 14, /* Length problem in SET_FIELD action. */
    OFPBAC_BAD_SET_ARGUMENT = 15, /* Bad argument in SET_FIELD action. */
};

/*For the OFPET_BAD_INSTRUCTION error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_BAD_INSTRUCTION. 'data' contains at least
* the first 64 bytes of the failed request. */
enum ofp_bad_instruction_code {
    OFPBIC_UNKNOWN_INST = 0, /* Unknown instruction. */
    OFPBIC_UNSUP_INST = 1, /* Switch or table does not support the
    instruction. */
    OFPBIC_BAD_TABLE_ID = 2, /* Invalid Table-ID specified. */
    OFPBIC_UNSUP_METADATA = 3, /* Metadata value unsupported by datapath. */
    OFPBIC_UNSUP_METADATA_MASK = 4, /* Metadata mask value unsupported by
    datapath. */
    OFPBIC_BAD_EXPERIMENTER = 5, /* Unknown experimenter id specified. */
    OFPBIC_BAD_EXP_TYPE = 6, /* Unknown instruction for experimenter id. */
    OFPBIC_BAD_LEN = 7, /* Length problem in instructions. */
    OFPBIC_EPERM = 8, /* Permissions error. */
};

/*For the OFPET_BAD_MATCH error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_BAD_MATCH. 'data' contains at least
* the first 64 bytes of the failed request. */
enum ofp_bad_match_code {
    OFPBMC_BAD_TYPE = 0, /* Unsupported match type specified by the
    match */
    OFPBMC_BAD_LEN = 1, /* Length problem in match. */
    OFPBMC_BAD_TAG = 2, /* Match uses an unsupported tag/encap. */
    OFPBMC_BAD_DL_ADDR_MASK = 3, /* Unsupported datalink addr mask - switch
    does not support arbitrary datalink
    address mask. */
    OFPBMC_BAD_NW_ADDR_MASK = 4, /* Unsupported network addr mask - switch
    does not support arbitrary network
    address mask. */
    OFPBMC_BAD_WILDCARDS = 5, /* Unsupported combination of fields masked
    or omitted in the match. */
    OFPBMC_BAD_FIELD = 6, /* Unsupported field type in the match. */
    OFPBMC_BAD_VALUE = 7, /* Unsupported value in a match field. */
    OFPBMC_BAD_MASK = 8, /* Unsupported mask specified in the match,
    97 . 2012; The Open Networking Foundation
    OpenFlow Switch Specication Version 1.3.1
    field is not dl-address or nw-address. */
    OFPBMC_BAD_PREREQ = 9, /* A prerequisite was not met. */
    OFPBMC_DUP_FIELD = 10, /* A field type was duplicated. */
    OFPBMC_EPERM = 11, /* Permissions error. */
};

/*For the OFPET_FLOW_MOD_FAILED error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_FLOW_MOD_FAILED. 'data' contains
* at least the first 64 bytes of the failed request. */
enum ofp_flow_mod_failed_code {
    OFPFMFC_UNKNOWN = 0, /* Unspecified error. */
    OFPFMFC_TABLE_FULL = 1, /* Flow not added because table was full. */
    OFPFMFC_BAD_TABLE_ID = 2, /* Table does not exist */
    OFPFMFC_OVERLAP = 3, /* Attempted to add overlapping flow with
    CHECK_OVERLAP flag set. */
    OFPFMFC_EPERM = 4, /* Permissions error. */
    OFPFMFC_BAD_TIMEOUT = 5, /* Flow not added because of unsupported
    idle/hard timeout. */
    OFPFMFC_BAD_COMMAND = 6, /* Unsupported or unknown command. */
    OFPFMFC_BAD_FLAGS = 7, /* Unsupported or unknown flags. */
};

/*For the OFPET_GROUP_MOD_FAILED error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_GROUP_MOD_FAILED. 'data' contains
* at least the first 64 bytes of the failed request. */
enum ofp_group_mod_failed_code {
    OFPGMFC_GROUP_EXISTS = 0, /* Group not added because a group ADD
    attempted to replace an
    already-present group. */
    OFPGMFC_INVALID_GROUP = 1, /* Group not added because Group
    specified is invalid. */
    OFPGMFC_WEIGHT_UNSUPPORTED = 2, /* Switch does not support unequal load
    sharing with select groups. */
    OFPGMFC_OUT_OF_GROUPS = 3, /* The group table is full. */
    OFPGMFC_OUT_OF_BUCKETS = 4, /* The maximum number of action buckets
    for a group has been exceeded. */
    OFPGMFC_CHAINING_UNSUPPORTED = 5, /* Switch does not support groups that
    forward to groups. */
    OFPGMFC_WATCH_UNSUPPORTED = 6, /* This group cannot watch the watch_port
    or watch_group specified. */
    OFPGMFC_LOOP = 7, /* Group entry would cause a loop. */
    OFPGMFC_UNKNOWN_GROUP = 8, /* Group not modified because a group
    MODIFY attempted to modify a
    non-existent group. */
    OFPGMFC_CHAINED_GROUP = 9, /* Group not deleted because another
    group is forwarding to it. */
    OFPGMFC_BAD_TYPE = 10, /* Unsupported or unknown group type. */
    OFPGMFC_BAD_COMMAND = 11, /* Unsupported or unknown command. */
    OFPGMFC_BAD_BUCKET = 12, /* Error in bucket. */
    OFPGMFC_BAD_WATCH = 13, /* Error in watch port/group. */
    OFPGMFC_EPERM = 14, /* Permissions error. */
};

/*For the OFPET_PORT_MOD_FAILED error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_PORT_MOD_FAILED. 'data' contains
* at least the first 64 bytes of the failed request. */
enum ofp_port_mod_failed_code {
    OFPPMFC_BAD_PORT = 0, /* Specified port number does not exist. */
    OFPPMFC_BAD_HW_ADDR = 1, /* Specified hardware address does not
    * match the port number. */
    OFPPMFC_BAD_CONFIG = 2, /* Specified config is invalid. */
    OFPPMFC_BAD_ADVERTISE = 3, /* Specified advertise is invalid. */
    OFPPMFC_EPERM = 4, /* Permissions error. */
};

/*For the OFPET_TABLE_MOD_FAILED error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_TABLE_MOD_FAILED. 'data' contains
* at least the first 64 bytes of the failed request. */
enum ofp_table_mod_failed_code {
    OFPTMFC_BAD_TABLE = 0, /* Specified table does not exist. */
    OFPTMFC_BAD_CONFIG = 1, /* Specified config is invalid. */
    OFPTMFC_EPERM = 2, /* Permissions error. */
};

/*For the OFPET_QUEUE_OP_FAILED error type, the following codes are currently dened:*/
/* ofp_error msg 'code' values for OFPET_QUEUE_OP_FAILED. 'data' contains
* at least the first 64 bytes of the failed request */
enum ofp_queue_op_failed_code {
    OFPQOFC_BAD_PORT = 0, /* Invalid port (or port does not exist). */
    OFPQOFC_BAD_QUEUE = 1, /* Queue does not exist. */
    OFPQOFC_EPERM = 2, /* Permissions error. */
};

/*For the OFPET_SWITCH_CONFIG_FAILED error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_SWITCH_CONFIG_FAILED. 'data' contains
* at least the first 64 bytes of the failed request. */
enum ofp_switch_config_failed_code {
    OFPSCFC_BAD_FLAGS = 0, /* Specified flags is invalid. */
    OFPSCFC_BAD_LEN = 1, /* Specified len is invalid. */
    OFPSCFC_EPERM = 2, /* Permissions error. */
};

/*For the OFPET_ROLE_REQUEST_FAILED error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_ROLE_REQUEST_FAILED. 'data' contains
* at least the first 64 bytes of the failed request. */
enum ofp_role_request_failed_code {
    OFPRRFC_STALE = 0, /* Stale Message: old generation_id. */
    OFPRRFC_UNSUP = 1, /* Controller role change unsupported. */
    OFPRRFC_BAD_ROLE = 2, /* Invalid role. */
};

/*For the OFPET_METER_MOD_FAILED error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_METER_MOD_FAILED. 'data' contains
* at least the first 64 bytes of the failed request. */
enum ofp_meter_mod_failed_code {
    OFPMMFC_UNKNOWN = 0, /* Unspecified error. */
    OFPMMFC_METER_EXISTS = 1, /* Meter not added because a Meter ADD
    * attempted to replace an existing Meter. */
    OFPMMFC_INVALID_METER = 2, /* Meter not added because Meter specified
    * is invalid. */
    OFPMMFC_UNKNOWN_METER = 3, /* Meter not modified because a Meter
    MODIFY attempted to modify a non-existent
    Meter. */
    OFPMMFC_BAD_COMMAND = 4, /* Unsupported or unknown command. */
    OFPMMFC_BAD_FLAGS = 5, /* Flag configuration unsupported. */
    OFPMMFC_BAD_RATE = 6, /* Rate unsupported. */
    OFPMMFC_BAD_BURST = 7, /* Burst size unsupported. */
    OFPMMFC_BAD_BAND = 8, /* Band unsupported. */
    OFPMMFC_BAD_BAND_VALUE = 9, /* Band value unsupported. */
    OFPMMFC_OUT_OF_METERS = 10, /* No more meters available. */
    OFPMMFC_OUT_OF_BANDS = 11, /* The maximum number of properties
    * for a meter has been exceeded. */
};

/*For the OFPET_TABLE_FEATURES_FAILED error type, the following codes are currently dened:*/
/* ofp_error_msg 'code' values for OFPET_TABLE_FEATURES_FAILED. 'data' contains
* at least the first 64 bytes of the failed request. */
enum ofp_table_features_failed_code {
    OFPTFFC_BAD_TABLE = 0, /* Specified table does not exist. */
    OFPTFFC_BAD_METADATA = 1, /* Invalid metadata mask. */
    OFPTFFC_BAD_TYPE = 2, /* Unknown property type. */
    OFPTFFC_BAD_LEN = 3, /* Length problem in properties. */
    OFPTFFC_BAD_ARGUMENT = 4, /* Unsupported property value. */
    OFPTFFC_EPERM = 5, /* Permissions error. */
};

/*For the OFPET_EXPERIMENTER error type, the error message is dened by the following structure and
elds, followed by experimenter dened data:*/
/* OFPET_EXPERIMENTER: Error message (datapath -> controller). */
struct ofp_error_experimenter_msg {
    struct ofp_header header;
    uint16_t type; /* OFPET_EXPERIMENTER. */
    uint16_t exp_type; /* Experimenter defined. */
    uint32_t experimenter; /* Experimenter ID which takes the same form
    as in struct ofp_experimenter_header. */
    uint8_t data[0]; /* Variable-length data. Interpreted based
    on the type and code. No padding. */
};
OFP_ASSERT(sizeof(struct ofp_error_experimenter_msg) == 16);
