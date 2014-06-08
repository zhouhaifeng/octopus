
/*A.3.5 Multipart Messages
While the system is running, the controller may request state from the datapath using the
OFPT_MULTIPART_REQUEST message:*/
struct ofp_multipart_request {
    struct ofp_header header;
    uint16_t type; /* One of the OFPMP_* constants. */
    uint16_t flags; /* OFPMPF_REQ_* flags. */
    uint8_t pad[4];
    uint8_t body[0]; /* Body of the request. */
};
OFP_ASSERT(sizeof(struct ofp_multipart_request) == 16);

enum ofp_multipart_request_flags {
    OFPMPF_REQ_MORE = 1 << 0 /* More requests to follow. */
};
/*The switch responds with one or more OFPT_MULTIPART_REPLY messages:*/
struct ofp_multipart_reply {
    struct ofp_header header;
    uint16_t type; /* One of the OFPMP_* constants. */
    uint16_t flags; /* OFPMPF_REPLY_* flags. */
    uint8_t pad[4];
    uint8_t body[0]; /* Body of the reply. */
};
OFP_ASSERT(sizeof(struct ofp_multipart_reply) == 16);

enum ofp_multipart_reply_flags {
    OFPMPF_REPLY_MORE = 1 << 0 /* More replies to follow. */
};

/*The only value dened for flags in a request and reply is whether more requests/replies will follow
this one - this has the value 0x0001. To ease implementation, the controller is allowed to send requests
and the switch is allowed to send replies with no additional entries. However, another message must
always follow a message with the more 
ag set. A request that spans multiple messages (has one
or more messages with the more 
ag set), must use the same transaction id (xid) for all messages
in the request. Messages from a multipart request may be interleaved with other OpenFlow message
types, including other multipart requests, but must have distinct transaction ids if multiple unanswered
multipart requests are in 
ight simultaneously. If a request spans multiple messages and grows to a
size that the switch is unable to buer, the switch should respond with an error messsage of type
OFPET_BAD_REQUEST and code OFPBRC_MULTIPART_BUFFER_OVERFLOW. Transaction ids of replies must
always match the request that prompted them.
In both the request and response, the type eld species the kind of information being passed and
determines how the body eld is interpreted:*/
enum ofp_multipart_types {
    /* Description of this OpenFlow switch.
    * The request body is empty.
    * The reply body is struct ofp_desc. */
    OFPMP_DESC = 0,
    /* Individual flow statistics.
    * The request body is struct ofp_flow_stats_request.
    * The reply body is an array of struct ofp_flow_stats. */
    OFPMP_FLOW = 1,
    /* Aggregate flow statistics.
    * The request body is struct ofp_aggregate_stats_request.
    * The reply body is struct ofp_aggregate_stats_reply. */
    OFPMP_AGGREGATE = 2,
    /* Flow table statistics.
    * The request body is empty.
    * The reply body is an array of struct ofp_table_stats. */
    OFPMP_TABLE = 3,
    /* Port statistics.
    * The request body is struct ofp_port_stats_request.
    * The reply body is an array of struct ofp_port_stats. */
    OFPMP_PORT_STATS = 4,
    /* Queue statistics for a port
    * The request body is struct ofp_queue_stats_request.
    * The reply body is an array of struct ofp_queue_stats */
    OFPMP_QUEUE = 5,
    /* Group counter statistics.
    * The request body is struct ofp_group_stats_request.
    * The reply is an array of struct ofp_group_stats. */
    OFPMP_GROUP = 6,
    /* Group description.
    * The request body is empty.
    * The reply body is an array of struct ofp_group_desc_stats. */
    OFPMP_GROUP_DESC = 7,
    /* Group features.
    * The request body is empty.
    * The reply body is struct ofp_group_features. */
    OFPMP_GROUP_FEATURES = 8,
    /* Meter statistics.
    * The request body is struct ofp_meter_multipart_requests.
    * The reply body is an array of struct ofp_meter_stats. */
    OFPMP_METER = 9,
    /* Meter configuration.
    * The request body is struct ofp_meter_multipart_requests.
    * The reply body is an array of struct ofp_meter_config. */
    OFPMP_METER_CONFIG = 10,
    /* Meter features.
    73 . 2012; The Open Networking Foundation
    OpenFlow Switch Specication Version 1.3.1
    * The request body is empty.
    * The reply body is struct ofp_meter_features. */
    OFPMP_METER_FEATURES = 11,
    /* Table features.
    * The request body is either empty or contains an array of
    * struct ofp_table_features containing the controller's
    * desired view of the switch. If the switch is unable to
    * set the specified view an error is returned.
    * The reply body is an array of struct ofp_table_features. */
    OFPMP_TABLE_FEATURES = 12,
    /* Port description.
    * The request body is empty.
    * The reply body is an array of struct ofp_port. */
    OFPMP_PORT_DESC = 13,
    /* Experimenter extension.
    * The request and reply bodies begin with
    * struct ofp_experimenter_multipart_header.
    * The request and reply bodies are otherwise experimenter-defined. */
    OFPMP_EXPERIMENTER = 0xffff
};
/*In all types of multipart reply containing statistics, if a specic numeric counter is not available in the
switch, its value must be set to the maximum eld value (the unsigned equivalent of -1). Counters are
unsigned and wrap around with no over
ow indicator.*/

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

A.3.5.2 Individual Flow Statistics
Information about individual 
ow entries is requested with the OFPMP_FLOW multipart request type:
/* Body for ofp_multipart_request of type OFPMP_FLOW. */
struct ofp_flow_stats_request {
uint8_t table_id; /* ID of table to read (from ofp_table_stats),
OFPTT_ALL for all tables. */
uint8_t pad[3]; /* Align to 32 bits. */
uint32_t out_port; /* Require matching entries to include this
as an output port. A value of OFPP_ANY
indicates no restriction. */
uint32_t out_group; /* Require matching entries to include this
as an output group. A value of OFPG_ANY
indicates no restriction. */
uint8_t pad2[4]; /* Align to 64 bits. */
uint64_t cookie; /* Require matching entries to contain this
cookie value */
uint64_t cookie_mask; /* Mask used to restrict the cookie bits that
must match. A value of 0 indicates
no restriction. */
struct ofp_match match; /* Fields to match. Variable size. */
};
OFP_ASSERT(sizeof(struct ofp_flow_stats_request) == 40);
The match eld contains a description of the 
ow entries that should be matched and may contain
wildcarded and masked elds. This eld's matching behavior is described in Section 6.4.
The table_id eld indicates the index of a single table to read, or OFPTT_ALL for all tables.
The out_port and out_group elds optionally lter by output port and group. If either out_port or
out_group contain a value other than OFPP_ANY and OFPG_ANY respectively, it introduces a constraint
when matching. This constraint is that the 
ow entry must contain an output action directed at that
port or group. Other constraints such as ofp_match structs are still used; this is purely an additional
constraint. Note that to disable output ltering, both out_port and out_group must be set to OFPP_ANY
and OFPG_ANY respectively.
The usage of the cookie and cookie_mask elds is dened in Section 6.4.
The body of the reply to a OFPMP_FLOW multipart request consists of an array of the following:
/* Body of reply to OFPMP_FLOW request. */
struct ofp_flow_stats {
uint16_t length; /* Length of this entry. */
uint8_t table_id; /* ID of table flow came from. */
uint8_t pad;
uint32_t duration_sec; /* Time flow has been alive in seconds. */
uint32_t duration_nsec; /* Time flow has been alive in nanoseconds beyond
duration_sec. */
uint16_t priority; /* Priority of the entry. */
uint16_t idle_timeout; /* Number of seconds idle before expiration. */
uint16_t hard_timeout; /* Number of seconds before expiration. */
uint16_t flags; /* One of OFPFF_*. */
uint8_t pad2[4]; /* Align to 64-bits. */
75 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
uint64_t cookie; /* Opaque controller-issued identifier. */
uint64_t packet_count; /* Number of packets in flow. */
uint64_t byte_count; /* Number of bytes in flow. */
struct ofp_match match; /* Description of fields. Variable size. */
//struct ofp_instruction instructions[0]; /* Instruction set. */
};
OFP_ASSERT(sizeof(struct ofp_flow_stats) == 56);
The elds consist of those provided in the flow_mod that created the 
ow entry (see A.3.4.1), plus
the table_id into which the entry was inserted, the packet_count, and the byte_count counting all
packets processed by the 
ow entry.
The duration_sec and duration_nsec elds indicate the elapsed time the 
ow entry has been installed
in the switch. The total duration in nanoseconds can be computed as duration sec109+duration nsec.
Implementations are required to provide second precision; higher precision is encouraged where available.
A.3.5.3 Aggregate Flow Statistics
Aggregate information about multiple 
ow entries is requested with the OFPMP_AGGREGATE multipart
request type:
/* Body for ofp_multipart_request of type OFPMP_AGGREGATE. */
struct ofp_aggregate_stats_request {
uint8_t table_id; /* ID of table to read (from ofp_table_stats)
OFPTT_ALL for all tables. */
uint8_t pad[3]; /* Align to 32 bits. */
uint32_t out_port; /* Require matching entries to include this
as an output port. A value of OFPP_ANY
indicates no restriction. */
uint32_t out_group; /* Require matching entries to include this
as an output group. A value of OFPG_ANY
indicates no restriction. */
uint8_t pad2[4]; /* Align to 64 bits. */
uint64_t cookie; /* Require matching entries to contain this
cookie value */
uint64_t cookie_mask; /* Mask used to restrict the cookie bits that
must match. A value of 0 indicates
no restriction. */
struct ofp_match match; /* Fields to match. Variable size. */
};
OFP_ASSERT(sizeof(struct ofp_aggregate_stats_request) == 40);
The elds in this message have the same meanings as in the individual 
ow stats request type
(OFPMP_FLOW).
The body of the reply consists of the following:
/* Body of reply to OFPMP_AGGREGATE request. */
struct ofp_aggregate_stats_reply {
uint64_t packet_count; /* Number of packets in flows. */
76 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
uint64_t byte_count; /* Number of bytes in flows. */
uint32_t flow_count; /* Number of flows. */
uint8_t pad[4]; /* Align to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp_aggregate_stats_reply) == 24);
A.3.5.4 Table Statistics
Information about tables is requested with the OFPMP_TABLE multipart request type. The request does
not contain any data in the body.
The body of the reply consists of an array of the following:
/* Body of reply to OFPMP_TABLE request. */
struct ofp_table_stats {
uint8_t table_id; /* Identifier of table. Lower numbered tables
are consulted first. */
uint8_t pad[3]; /* Align to 32-bits. */
uint32_t active_count; /* Number of active entries. */
uint64_t lookup_count; /* Number of packets looked up in table. */
uint64_t matched_count; /* Number of packets that hit table. */
};
OFP_ASSERT(sizeof(struct ofp_table_stats) == 24);
The array has one structure for each table supported by the switch. The entries are returned in the
order that packets traverse the tables.
A.3.5.5 Table Features
The OFPMP_TABLE_FEATURES multipart type allows a controller to both query for the capabilities of
existing tables, and to optionally ask the switch to recongure its tables to match a supplied conguration.
In general, the table feature capabilities represents all possible features of a table, however some
features may be mutually exclusive and the current capabilities structures do not allow us to represent
such exclusions.
If the OFPMP_TABLE_FEATURES request body is empty the switch will return an array of struct
ofp_table_features containing the capabilities of the currently congured tables. If the request
body contains an array of struct ofp_table_features the switch will attempt to change its tables to
match the requested table conguration. A successful conguration will contain all tables from the
request, and each table's capabilities will either be a superset of, or equal to the requested capabilities.
If the table conguration is successful, 
ows from all tables in the prior conguration are expired, and
expirations sent if requested. The switch then replies with the new conguration, in the same order
as the request where possible. If the switch is unable to set the requested conguration, an error of
type OFPET_TABLE_FEATURES_FAILED is returned, and the current table conguration is returned in the
multipart reply.
The following structure describes the body of the table features request and reply:
77 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
/* Body for ofp_multipart_request of type OFPMP_TABLE_FEATURES./
* Body of reply to OFPMP_TABLE_FEATURES request. */
struct ofp_table_features {
uint16_t length; /* Length is padded to 64 bits. */
uint8_t table_id; /* Identifier of table. Lower numbered tables
are consulted first. */
uint8_t pad[5]; /* Align to 64-bits. */
char name[OFP_MAX_TABLE_NAME_LEN];
uint64_t metadata_match; /* Bits of metadata table can match. */
uint64_t metadata_write; /* Bits of metadata table can write. */
uint32_t config; /* Bitmap of OFPTC_* values */
uint32_t max_entries; /* Max number of entries supported. */
/* Table Feature Property list */
struct ofp_table_feature_prop_header properties[0];
};
OFP_ASSERT(sizeof(struct ofp_table_features) == 64);
The array has one structure for each table supported by the switch. The entries are returned in the
order that packets traverse the tables. OFP_MAX_TABLE_NAME_LEN is 32 .
The metadata_match eld indicates the bits of the metadata eld that the table can match on, when
using the metadata eld of struct ofp_match. A value of 0xFFFFFFFFFFFFFFFF indicates that the
table can match the full metadata eld.
The metadata_write eld indicates the bits of the metadata eld that the table can write using the
OFPIT_WRITE_METADATA instruction. A value of 0xFFFFFFFFFFFFFFFF indicates that the table can write
the full metadata eld.
The config eld is the table conguration that was set on the table via a table conguration message
(see A.3.3).
The max_entries eld describes the maximum number of 
ow entries that can be inserted into that
table. Due to limitations imposed by modern hardware, the max_entries value should be considered
advisory and a best eort approximation of the capacity of the table. Despite the high-level abstraction
of a table, in practice the resource consumed by a single 
ow table entry is not constant. For example,
a 
ow table entry might consume more than one entry, depending on its match parameters (e.g.,
IPv4 vs. IPv6). Also, tables that appear distinct at an OpenFlow-level might in fact share the same
underlying physical resources. Further, on OpenFlow hybrid switches, those table may be shared with
non-OpenFlow functions. The result is that switch implementers should report an approximation of
the total 
ow entries supported and controller writers should not treat this value as a xed, physical
constant.
The properties eld is a list of table feature properties, describing various capabilities of the table.
The list of table feature property types that are currently dened are:
/* Table Feature property types.
* Low order bit cleared indicates a property for a regular Flow Entry.
* Low order bit set indicates a property for the Table-Miss Flow Entry.
*/
enum ofp_table_feature_prop_type {
OFPTFPT_INSTRUCTIONS = 0, /* Instructions property. */
OFPTFPT_INSTRUCTIONS_MISS = 1, /* Instructions for table-miss. */
78 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
OFPTFPT_NEXT_TABLES = 2, /* Next Table property. */
OFPTFPT_NEXT_TABLES_MISS = 3, /* Next Table for table-miss. */
OFPTFPT_WRITE_ACTIONS = 4, /* Write Actions property. */
OFPTFPT_WRITE_ACTIONS_MISS = 5, /* Write Actions for table-miss. */
OFPTFPT_APPLY_ACTIONS = 6, /* Apply Actions property. */
OFPTFPT_APPLY_ACTIONS_MISS = 7, /* Apply Actions for table-miss. */
OFPTFPT_MATCH = 8, /* Match property. */
OFPTFPT_WILDCARDS = 10, /* Wildcards property. */
OFPTFPT_WRITE_SETFIELD = 12, /* Write Set-Field property. */
OFPTFPT_WRITE_SETFIELD_MISS = 13, /* Write Set-Field for table-miss. */
OFPTFPT_APPLY_SETFIELD = 14, /* Apply Set-Field property. */
OFPTFPT_APPLY_SETFIELD_MISS = 15, /* Apply Set-Field for table-miss. */
OFPTFPT_EXPERIMENTER = 0xFFFE, /* Experimenter property. */
OFPTFPT_EXPERIMENTER_MISS = 0xFFFF, /* Experimenter for table-miss. */
};
The properties with the _MISS sux describe the capabilities for the table-miss 
ow entry (see 5.4),
whereas other properties describe the capabilities for regular 
ow entry. If a specic property does not
have any capability (for example no Set-Field support), a property with an empty list must be included
in the property list. When a property of the table-miss 
ow entry is the same as the corresponding
property for regular 
ow entries (i.e. both properties have the same list of capabilities), this table-miss
property can be omited from the property list.
A property denition contains the property type, length, and any associated data:
/* Common header for all Table Feature Properties */
struct ofp_table_feature_prop_header {
uint16_t type; /* One of OFPTFPT_*. */
uint16_t length; /* Length in bytes of this property. */
};
OFP_ASSERT(sizeof(struct ofp_table_feature_prop_header) == 4);
The OFPTFPT_INSTRUCTIONS and OFPTFPT_INSTRUCTIONS_MISS properties use the following structure
and elds:
/* Instructions property */
struct ofp_table_feature_prop_instructions {
uint16_t type; /* One of OFPTFPT_INSTRUCTIONS,
OFPTFPT_INSTRUCTIONS_MISS. */
uint16_t length; /* Length in bytes of this property. */
/* Followed by:
* - Exactly (length - 4) bytes containing the instruction ids, then
* - Exactly (length + 7)/8*8 - (length) (between 0 and 7)
* bytes of all-zero bytes */
struct ofp_instruction instruction_ids[0]; /* List of instructions */
};
OFP_ASSERT(sizeof(struct ofp_table_feature_prop_instructions) == 4);
The instruction_ids is the list of instructions supported by this table (see 5.9). The elements of that
list are variable size to enable expressing experimenter instructions, non-experimenter instructions are
4 bytes.
79 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
The OFPTFPT_NEXT_TABLES and OFPTFPT_NEXT_TABLES_MISS properties use the following structure and
elds:
/* Next Tables property */
struct ofp_table_feature_prop_next_tables {
uint16_t type; /* One of OFPTFPT_NEXT_TABLES,
OFPTFPT_NEXT_TABLES_MISS. */
uint16_t length; /* Length in bytes of this property. */
/* Followed by:
* - Exactly (length - 4) bytes containing the table_ids, then
* - Exactly (length + 7)/8*8 - (length) (between 0 and 7)
* bytes of all-zero bytes */
uint8_t next_table_ids[0];
};
OFP_ASSERT(sizeof(struct ofp_table_feature_prop_next_tables) == 4);
The next_table_ids is the array of tables that can be directly reached from the present table using
the OFPIT_GOTO_TABLE instruction (see 5.1).
The OFPTFPT_WRITE_ACTIONS, OFPTFPT_WRITE_ACTIONS_MISS, OFPTFPT_APPLY_ACTIONS and
OFPTFPT_APPLY_ACTIONS_MISS properties use the following structure and elds:
/* Actions property */
struct ofp_table_feature_prop_actions {
uint16_t type; /* One of OFPTFPT_WRITE_ACTIONS,
OFPTFPT_WRITE_ACTIONS_MISS,
OFPTFPT_APPLY_ACTIONS,
OFPTFPT_APPLY_ACTIONS_MISS. */
uint16_t length; /* Length in bytes of this property. */
/* Followed by:
* - Exactly (length - 4) bytes containing the action_ids, then
* - Exactly (length + 7)/8*8 - (length) (between 0 and 7)
* bytes of all-zero bytes */
struct ofp_action_header action_ids[0]; /* List of actions */
};
OFP_ASSERT(sizeof(struct ofp_table_feature_prop_actions) == 4);
The action_ids is the list of actions for the feature (see 5.12). The elements of that list are
variable size to enable expressing experimenter actions, non-experimenter actions are 4 bytes. The
OFPTFPT_WRITE_ACTIONS and OFPTFPT_WRITE_ACTIONS_MISS properties describe actions supported
by the table using the OFPIT_WRITE_ACTIONS instruction, whereas the OFPTFPT_APPLY_ACTIONS
and OFPTFPT_APPLY_ACTIONS_MISS properties describe actions supported by the table using the
OFPIT_APPLY_ACTIONS instruction.
The OFPTFPT_MATCH, OFPTFPT_WILDCARDS, OFPTFPT_WRITE_SETFIELD,
OFPTFPT_WRITE_SETFIELD_MISS, OFPTFPT_APPLY_SETFIELD and OFPTFPT_APPLY_SETFIELD_MISS
properties use the following structure and elds:
/* Match, Wildcard or Set-Field property */
struct ofp_table_feature_prop_oxm {
uint16_t type; /* One of OFPTFPT_MATCH,
OFPTFPT_WILDCARDS,
80 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
OFPTFPT_WRITE_SETFIELD,
OFPTFPT_WRITE_SETFIELD_MISS,
OFPTFPT_APPLY_SETFIELD,
OFPTFPT_APPLY_SETFIELD_MISS. */
uint16_t length; /* Length in bytes of this property. */
/* Followed by:
* - Exactly (length - 4) bytes containing the oxm_ids, then
* - Exactly (length + 7)/8*8 - (length) (between 0 and 7)
* bytes of all-zero bytes */
uint32_t oxm_ids[0]; /* Array of OXM headers */
};
OFP_ASSERT(sizeof(struct ofp_table_feature_prop_oxm) == 4);
The oxm_ids is the list of OXM types for the feature (see A.2.3.2). The elements of that list are 32-bit
OXM headers or 64-bit OXM headers for experimenter OXM elds.
The OFPTFPT_MATCH property indicates the elds for which that particular table supports matching on
(see A.2.3.7). For example, if the table can match the ingress port, an OXM header with the type
OFPXMT_OFB_IN_PORT should be included in the list. If the HASMASK bit is set on the OXM header then
the switch must support masking for the given type. The OFPTFPT_WILDCARDS property indicates the
elds for which that particular table supports wildcarding (omiting). For example, a direct look-up
hash table would have that list empty, while a TCAM or sequentially searched table would have it set
to the same value as the OFPTFPT_MATCH property.
The OFPTFPT_WRITE_SETFIELD and OFPTFPT_WRITE_SETFIELD_MISS properties describe Set-Field
action types supported by the table using the OFPIT_WRITE_ACTIONS instruction, whereas the
OFPTFPT_APPLY_SETFIELD and OFPTFPT_APPLY_SETFIELD_MISS properties describe Set-Field action
types supported by the table using the OFPIT_APPLY_ACTIONS instruction.
All elds in ofp_table_features may be requested to be changed by the controller with the exception
of the max_entries eld, this is read only and returned by the switch.
The OFPTFPT_APPLY_ACTIONS, OFPTFPT_APPLY_ACTIONS_MISS, OFPTFPT_APPLY_SETFIELD, and
OFPTFPT_APPLY_SETFIELD_MISS properties contain actions and elds the table is capable of applying.
For each of these lists, if an element is present it means the table is at least capable of applying the
element in isolation one time. There is currently no way to indicate which elements can be applied
together, in which order, and how many time an element can be applied in a single 
ow entry.
The OFPTFPT_EXPERIMENTER and OFPTFPT_EXPERIMENTER_MISS properties uses the following structure
and elds:
/* Experimenter table feature property */
struct ofp_table_feature_prop_experimenter {
uint16_t type; /* One of OFPTFPT_EXPERIMENTER,
OFPTFPT_EXPERIMENTER_MISS. */
uint16_t length; /* Length in bytes of this property. */
uint32_t experimenter; /* Experimenter ID which takes the same
form as in struct
ofp_experimenter_header. */
uint32_t exp_type; /* Experimenter defined. */
/* Followed by:
* - Exactly (length - 12) bytes containing the experimenter data, then
* - Exactly (length + 7)/8*8 - (length) (between 0 and 7)
81 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
* bytes of all-zero bytes */
uint32_t experimenter_data[0];
};
OFP_ASSERT(sizeof(struct ofp_table_feature_prop_experimenter) == 12);
The experimenter eld is the Experimenter ID, which takes the same form as in struct
ofp_experimenter (see A.5.4).
A.3.5.6 Port Statistics
Information about ports statistics is requested with the OFPMP_PORT_STATS multipart request type:
/* Body for ofp_multipart_request of type OFPMP_PORT. */
struct ofp_port_stats_request {
uint32_t port_no; /* OFPMP_PORT message must request statistics
* either for a single port (specified in
* port_no) or for all ports (if port_no ==
* OFPP_ANY). */
uint8_t pad[4];
};
OFP_ASSERT(sizeof(struct ofp_port_stats_request) == 8);
The port_no eld optionally lters the stats request to the given port. To request all port statistics,
port_no must be set to OFPP_ANY.
The body of the reply consists of an array of the following:
/* Body of reply to OFPMP_PORT request. If a counter is unsupported, set
* the field to all ones. */
struct ofp_port_stats {
uint32_t port_no;
uint8_t pad[4]; /* Align to 64-bits. */
uint64_t rx_packets; /* Number of received packets. */
uint64_t tx_packets; /* Number of transmitted packets. */
uint64_t rx_bytes; /* Number of received bytes. */
uint64_t tx_bytes; /* Number of transmitted bytes. */
uint64_t rx_dropped; /* Number of packets dropped by RX. */
uint64_t tx_dropped; /* Number of packets dropped by TX. */
uint64_t rx_errors; /* Number of receive errors. This is a super-set
of more specific receive errors and should be
greater than or equal to the sum of all
rx_*_err values. */
uint64_t tx_errors; /* Number of transmit errors. This is a super-set
of more specific transmit errors and should be
greater than or equal to the sum of all
tx_*_err values (none currently defined.) */
uint64_t rx_frame_err; /* Number of frame alignment errors. */
uint64_t rx_over_err; /* Number of packets with RX overrun. */
uint64_t rx_crc_err; /* Number of CRC errors. */
uint64_t collisions; /* Number of collisions. */
uint32_t duration_sec; /* Time port has been alive in seconds. */
uint32_t duration_nsec; /* Time port has been alive in nanoseconds beyond
82 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
duration_sec. */
};
OFP_ASSERT(sizeof(struct ofp_port_stats) == 112);
The duration_sec and duration_nsec elds indicate the elapsed time the port has been congured
into the OpenFlow pipeline. The total duration in nanoseconds can be computed as duration sec 
109 + duration nsec. Implementations are required to provide second precision; higher precision is
encouraged where available.
A.3.5.7 Port Description
The port description request OFPMP_PORT_DESCRIPTION enables the controller to get a description of all
the ports in the system that support OpenFlow. The request body is empty. The reply body consists
of an array of the following:
/* Description of a port */
struct ofp_port {
uint32_t port_no;
uint8_t pad[4];
uint8_t hw_addr[OFP_ETH_ALEN];
uint8_t pad2[2]; /* Align to 64 bits. */
char name[OFP_MAX_PORT_NAME_LEN]; /* Null-terminated */
uint32_t config; /* Bitmap of OFPPC_* flags. */
uint32_t state; /* Bitmap of OFPPS_* flags. */
/* Bitmaps of OFPPF_* that describe features. All bits zeroed if
* unsupported or unavailable. */
uint32_t curr; /* Current features. */
uint32_t advertised; /* Features being advertised by the port. */
uint32_t supported; /* Features supported by the port. */
uint32_t peer; /* Features advertised by peer. */
uint32_t curr_speed; /* Current port bitrate in kbps. */
uint32_t max_speed; /* Max port bitrate in kbps */
};
OFP_ASSERT(sizeof(struct ofp_port) == 64);
This structure is described in section A.2.1.
A.3.5.8 Queue Statistics
The OFPMP_QUEUE multipart request message provides queue statistics for one or more ports and one
or more queues. The request body contains a port_no eld identifying the OpenFlow port for which
statistics are requested, or OFPP_ANY to refer to all ports. The queue_id eld identies one of the priority
queues, or OFPQ_ALL to refer to all queues congured at the specied port. OFPQ_ALL is 0x.
83 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
struct ofp_queue_stats_request {
uint32_t port_no; /* All ports if OFPP_ANY. */
uint32_t queue_id; /* All queues if OFPQ_ALL. */
};
OFP_ASSERT(sizeof(struct ofp_queue_stats_request) == 8);
The body of the reply consists of an array of the following structure:
struct ofp_queue_stats {
uint32_t port_no;
uint32_t queue_id; /* Queue i.d */
uint64_t tx_bytes; /* Number of transmitted bytes. */
uint64_t tx_packets; /* Number of transmitted packets. */
uint64_t tx_errors; /* Number of packets dropped due to overrun. */
uint32_t duration_sec; /* Time queue has been alive in seconds. */
uint32_t duration_nsec; /* Time queue has been alive in nanoseconds beyond
duration_sec. */
};
OFP_ASSERT(sizeof(struct ofp_queue_stats) == 40);
The duration_sec and duration_nsec elds indicate the elapsed time the queue has been installed in
the switch. The total duration in nanoseconds can be computed as duration sec109+duration nsec.
Implementations are required to provide second precision; higher precision is encouraged where available.
A.3.5.9 Group Statistics
The OFPMP_GROUP multipart request message provides statistics for one or more groups. The request
body consists of a group_id eld, which can be set to OFPG_ALL to refer to all groups on the switch.
/* Body of OFPMP_GROUP request. */
struct ofp_group_stats_request {
uint32_t group_id; /* All groups if OFPG_ALL. */
uint8_t pad[4]; /* Align to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp_group_stats_request) == 8);
The body of the reply consists of an array of the following structure:
/* Body of reply to OFPMP_GROUP request. */
struct ofp_group_stats {
uint16_t length; /* Length of this entry. */
uint8_t pad[2]; /* Align to 64 bits. */
uint32_t group_id; /* Group identifier. */
uint32_t ref_count; /* Number of flows or groups that directly forward
to this group. */
uint8_t pad2[4]; /* Align to 64 bits. */
uint64_t packet_count; /* Number of packets processed by group. */
uint64_t byte_count; /* Number of bytes processed by group. */
uint32_t duration_sec; /* Time group has been alive in seconds. */
84 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
uint32_t duration_nsec; /* Time group has been alive in nanoseconds beyond
duration_sec. */
struct ofp_bucket_counter bucket_stats[0];
};
OFP_ASSERT(sizeof(struct ofp_group_stats) == 40);
The elds consist of those provided in the group_mod that created the group, plus the ref_count
counting the number of 
ows referencing directly the group, the packet_count, and the byte_count
counting all packets processed by the group.
The duration_sec and duration_nsec elds indicate the elapsed time the group has been installed in
the switch. The total duration in nanoseconds can be computed as duration sec109+duration nsec.
Implementations are required to provide second precision; higher precision is encouraged where available.
The bucket_stats eld consists of an array of ofp_bucket_counter structs:
/* Used in group stats replies. */
struct ofp_bucket_counter {
uint64_t packet_count; /* Number of packets processed by bucket. */
uint64_t byte_count; /* Number of bytes processed by bucket. */
};
OFP_ASSERT(sizeof(struct ofp_bucket_counter) == 16);
A.3.5.10 Group Description
The OFPMP_GROUP_DESC multipart request message provides a way to list the set of groups on a switch,
along with their corresponding bucket actions. The request body is empty, while the reply body is an
array of the following structure:
/* Body of reply to OFPMP_GROUP_DESC request. */
struct ofp_group_desc_stats {
uint16_t length; /* Length of this entry. */
uint8_t type; /* One of OFPGT_*. */
uint8_t pad; /* Pad to 64 bits. */
uint32_t group_id; /* Group identifier. */
struct ofp_bucket buckets[0];
};
OFP_ASSERT(sizeof(struct ofp_group_desc_stats) == 8);
Fields for group description are the same as those used with the ofp_group_mod struct (see A.3.4.2).
A.3.5.11 Group Features
The OFPMP_GROUP_FEATURES multipart request message provides a way to list the capabilities of groups
on a switch. The request body is empty, while the reply body is the following structure:
85 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
/* Body of reply to OFPMP_GROUP_FEATURES request. Group features. */
struct ofp_group_features {
uint32_t types; /* Bitmap of OFPGT_* values supported. */
uint32_t capabilities; /* Bitmap of OFPGFC_* capability supported. */
uint32_t max_groups[4]; /* Maximum number of groups for each type. */
uint32_t actions[4]; /* Bitmaps of OFPAT_* that are supported. */
};
OFP_ASSERT(sizeof(struct ofp_group_features) == 40);
The max_groups eld is the maximum number of groups for each type of groups. The actions is the
supported actions for each type of groups. The capabilities uses the following 
ags:
/* Group configuration flags */
enum ofp_group_capabilities {
OFPGFC_SELECT_WEIGHT = 1 << 0, /* Support weight for select groups */
OFPGFC_SELECT_LIVENESS = 1 << 1, /* Support liveness for select groups */
OFPGFC_CHAINING = 1 << 2, /* Support chaining groups */
OFPGFC_CHAINING_CHECKS = 1 << 3, /* Check chaining for loops and delete */
};
A.3.5.12 Meter Statistics
The OFPMT_METER stats request message provides statistics for one or more meters. The request body
consists of a meter_id eld, which can be set to OFPM_ALL to refer to all meters on the switch.
/* Body of OFPMP_METER and OFPMP_METER_CONFIG requests. */
struct ofp_meter_multipart_request {
uint32_t meter_id; /* Meter instance, or OFPM_ALL. */
uint8_t pad[4]; /* Align to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp_meter_multipart_request) == 8);
The body of the reply consists of an array of the following structure:
/* Body of reply to OFPMP_METER request. Meter statistics. */
struct ofp_meter_stats {
uint32_t meter_id; /* Meter instance. */
uint16_t len; /* Length in bytes of this stats. */
uint8_t pad[6];
uint32_t flow_count; /* Number of flows bound to meter. */
uint64_t packet_in_count; /* Number of packets in input. */
uint64_t byte_in_count; /* Number of bytes in input. */
uint32_t duration_sec; /* Time meter has been alive in seconds. */
uint32_t duration_nsec; /* Time meter has been alive in nanoseconds beyond
duration_sec. */
struct ofp_meter_band_stats band_stats[0]; /* The band_stats length is
inferred from the length field. */
};
OFP_ASSERT(sizeof(struct ofp_meter_stats) == 40);
86 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
The packet_in_count, and the byte_in_count count all packets processed by the meter.
The duration_sec and duration_nsec elds indicate the elapsed time the meter has been installed in
the switch. The total duration in nanoseconds can be computed as duration sec109+duration nsec.
Implementations are required to provide second precision; higher precision is encouraged where available.
The band_stats eld consists of an array of ofp_meter_band_stats structs:
/* Statistics for each meter band */
struct ofp_meter_band_stats {
uint64_t packet_band_count; /* Number of packets in band. */
uint64_t byte_band_count; /* Number of bytes in band. */
};
OFP_ASSERT(sizeof(struct ofp_meter_band_stats) == 16);
The packet_band_count, and the byte_band_count count all packets processed by the band.
The order of the band statistics must be the same as in the OFPMT_METER_CONFIG stats reply.
A.3.5.13 Meter Conguration Statistics
The OFPMT_METER_CONFIG stats request message provides conguration for one or more meter. The
request body consists of a meter_id eld, which can be set to OFPM_ALL to refer to all meters on the
switch.
/* Body of OFPMP_METER and OFPMP_METER_CONFIG requests. */
struct ofp_meter_multipart_request {
uint32_t meter_id; /* Meter instance, or OFPM_ALL. */
uint8_t pad[4]; /* Align to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp_meter_multipart_request) == 8);
The body of the reply consists of an array of the following structure:
/* Body of reply to OFPMP_METER_CONFIG request. Meter configuration. */
struct ofp_meter_config {
uint16_t length; /* Length of this entry. */
uint16_t flags; /* All OFPMC_* that apply. */
uint32_t meter_id; /* Meter instance. */
struct ofp_meter_band_header bands[0]; /* The bands length is
inferred from the length field. */
};
OFP_ASSERT(sizeof(struct ofp_meter_config) == 8);
The elds are the same elds used for conguring the meter (see A.3.4.4).
87 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
A.3.5.14 Meter Features Statistics
The OFPMT_METER_FEATURES stats request message provides the set of features of the metering subsystem.
The request body is empty, and the body of the reply consists of the following structure:
/* Body of reply to OFPMP_METER_FEATURES request. Meter features. */
struct ofp_meter_features {
uint32_t max_meter; /* Maximum number of meters. */
uint32_t band_types; /* Bitmaps of OFPMBT_* values supported. */
uint32_t capabilities; /* Bitmaps of "ofp_meter_flags". */
uint8_t max_bands; /* Maximum bands per meters */
uint8_t max_color; /* Maximum color value */
uint8_t pad[2];
};
OFP_ASSERT(sizeof(struct ofp_meter_features) == 16);
A.3.5.15 Experimenter Multipart
Experimenter-specic multipart messages are requested with the OFPMP_EXPERIMENTER multipart type.
The rst bytes of the request and reply bodies are the following structure:
/* Body for ofp_multipart_request/reply of type OFPMP_EXPERIMENTER. */
struct ofp_experimenter_multipart_header {
uint32_t experimenter; /* Experimenter ID which takes the same form
as in struct ofp_experimenter_header. */
uint32_t exp_type; /* Experimenter defined. */
/* Experimenter-defined arbitrary additional data. */
};
OFP_ASSERT(sizeof(struct ofp_experimenter_multipart_header) == 8);
The rest of the request and reply bodies are experimenter-dened.
The experimenter eld is the Experimenter ID, which takes the same form as in struct
ofp_experimenter (see A.5.4).
A.3.6 Queue Conguration Messages
Queue conguration takes place outside the OpenFlow protocol, either through a command line tool or
through an external dedicated conguration protocol.
The controller can query the switch for congured queues on a port using the following structure:
/* Query for port queue configuration. */
struct ofp_queue_get_config_request {
struct ofp_header header;
uint32_t port; /* Port to be queried. Should refer
to a valid physical port (i.e. < OFPP_MAX),
or OFPP_ANY to request all configured
queues.*/
uint8_t pad[4];
};
OFP_ASSERT(sizeof(struct ofp_queue_get_config_request) == 16);
88 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
The switch replies back with an ofp_queue_get_config_reply command, containing a list of congured
queues.
/* Queue configuration for a given port. */
struct ofp_queue_get_config_reply {
struct ofp_header header;
uint32_t port;
uint8_t pad[4];
struct ofp_packet_queue queues[0]; /* List of configured queues. */
};
OFP_ASSERT(sizeof(struct ofp_queue_get_config_reply) == 16);
A.3.7 Packet-Out Message
When the controller wishes to send a packet out through the datapath, it uses the OFPT_PACKET_OUT
message:
/* Send packet (controller -> datapath). */
struct ofp_packet_out {
struct ofp_header header;
uint32_t buffer_id; /* ID assigned by datapath (OFP_NO_BUFFER
if none). */
uint32_t in_port; /* Packet's input port or OFPP_CONTROLLER. */
uint16_t actions_len; /* Size of action array in bytes. */
uint8_t pad[6];
struct ofp_action_header actions[0]; /* Action list. */
/* uint8_t data[0]; */ /* Packet data. The length is inferred
from the length field in the header.
(Only meaningful if buffer_id == -1.) */
};
OFP_ASSERT(sizeof(struct ofp_packet_out) == 24);
The buffer_id is the same given in the ofp_packet_in message. If the buffer_id is OFP_NO_BUFFER,
then the packet data is included in the data array. OFP_NO_BUFFER is 0x.
The in_port eld is the ingress port that must be associated with the packet for OpenFlow processing.
It must be set to either a valid standard switch port or OFPP_CONTROLLER.
The action eld is an action list dening how the packet should be processed by the switch. It may include
packet modication, group processing and an output port. The action list of an OFPT_PACKET_OUT
message can also specify the OFPP_TABLE reserved port as an output action to process the packet through
the existing 
ow entries, starting at the rst 
ow table. If OFPP_TABLE is specied, the in_port eld is
used as the ingress port in the 
ow table lookup.
In some cases, packets sent to OFPP_TABLE may be forwarded back to the controller as the result of a 
ow
entry action or table miss. Detecting and taking action for such controller-to-switch loops is outside the
scope of this specication. In general, OpenFlow messages are not guaranteed to be processed in order,
therefore if a OFPT_PACKET_OUT message using OFPP_TABLE depends on a 
ow that was recently sent to
the switch (with a OFPT_FLOW_MOD message), a OFPT_BARRIER_REQUEST message may be required prior
to the OFPT_PACKET_OUT message to make sure the 
ow entry was committed to the 
ow table prior to
execution of OFPP_TABLE.
89 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
A.3.8 Barrier Message
When the controller wants to ensure message dependencies have been met or wants to receive notications
for completed operations, it may use an OFPT_BARRIER_REQUEST message. This message has no
body. Upon receipt, the switch must nish processing all previously-received messages, including sending
corresponding reply or error messages, before executing any messages beyond the Barrier Request.
When such processing is complete, the switch must send an OFPT_BARRIER_REPLY message with the xid
of the original request.
A.3.9 Role Request Message
When the controller wants to change its role, it uses the OFPT_ROLE_REQUEST message with the following
structure:
/* Role request and reply message. */
struct ofp_role_request {
struct ofp_header header; /* Type OFPT_ROLE_REQUEST/OFPT_ROLE_REPLY. */
uint32_t role; /* One of NX_ROLE_*. */
uint8_t pad[4]; /* Align to 64 bits. */
uint64_t generation_id; /* Master Election Generation Id */
};
OFP_ASSERT(sizeof(struct ofp_role_request) == 24);
The eld role is the new role that the controller wants to assume, and can have the following values:
/* Controller roles. */
enum ofp_controller_role {
OFPCR_ROLE_NOCHANGE = 0, /* Don't change current role. */
OFPCR_ROLE_EQUAL = 1, /* Default role, full access. */
OFPCR_ROLE_MASTER = 2, /* Full access, at most one master. */
OFPCR_ROLE_SLAVE = 3, /* Read-only access. */
};
If the role value is OFPCR_ROLE_MASTER, all other controllers which role was OFPCR_ROLE_MASTER are
changed to OFPCR_ROLE_SLAVE. If the role value is OFPCR_ROLE_NOCHANGE, the current role of the controller
is not changed ; this enable a controller to query its current role without changing it.
Upon receipt of a OFPT_ROLE_REQUEST message, the switch must return a OFPT_ROLE_REPLY message.
The structure of this message is exactly the same as the OFPT_ROLE_REQUEST message, and the eld
role is the current role of the controller.
Additionally, if the role value in the message is OFPCR_ROLE_MASTER or OFPCR_ROLE_SLAVE, the switch
must validate generation_id to check for stale messages. If the validation fails, the switch must
discard the role request and return an error message with type OFPET_ROLE_REQUEST_FAILED and code
OFPRRFC_STALE.
90 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
A.3.10 Set Asynchronous Conguration Message
The controller is able to set and query the asynchronous messages that it wants to receive (other than
error messages) on a given OpenFlow channel with the OFPT_SET_ASYNC and OFPT_GET_ASYNC_REQUEST
messages, respectively. The switch responds to a OFPT_GET_ASYNC_REQUEST message with an
OFPT_GET_ASYNC_REPLY message; it does not reply to a request to set the conguration.
There is no body for OFPT_GET_ASYNC_REQUEST beyond the OpenFlow header. The OFPT_SET_ASYNC
and OFPT_GET_ASYNC_REPLY messages have the following format:
/* Asynchronous message configuration. */
struct ofp_async_config {
struct ofp_header header; /* OFPT_GET_ASYNC_REPLY or OFPT_SET_ASYNC. */
uint32_t packet_in_mask[2]; /* Bitmasks of OFPR_* values. */
uint32_t port_status_mask[2]; /* Bitmasks of OFPPR_* values. */
uint32_t flow_removed_mask[2];/* Bitmasks of OFPRR_* values. */
};
OFP_ASSERT(sizeof(struct ofp_async_config) == 32);
struct ofp_async_config contains three 2-element arrays. Each array controls whether the controller
receives asynchronous messages with a specic enum ofp_type. Within each array, element 0 species
messages of interest when the controller has a OFPCR_ROLE_EQUAL or OFPCR_ROLE_MASTER role; element
1, when the controller has a OFPCR_ROLE_SLAVE role. Each array element is a bit-mask in which a
0-bit disables receiving a message sent with the reason code corresponding to the bit index and a
1-bit enables receiving it. For example, the bit with value 22 = 4 in port_status_mask[1] determines
whether the controller will receive OFPT_PORT_STATUS messages with reason OFPPR_MODIFY (value 2)
when the controller has role OFPCR_ROLE_SLAVE.
OFPT_SET_ASYNC sets whether a controller should receive a given asynchronous message that is generated
by the switch. Other OpenFlow features control whether a given message is generated; for example,
the OFPFF_SEND_FLOW_REM 
ag controls whether the switch generates OFPT_FLOW_REMOVED a message
when a 
ow entry is removed.
A switch conguration, for example using the OpenFlow Conguration Protocol, may set the initial
conguration of asynchronous messages when an OpenFlow connection is established. In the absence
of such switch conguration, the initial conguration shall be:
. In the \master" or \equal" role, enable all OFPT_PACKET_IN messages, except those with reason
OFPR_INVALID_TTL, and enable all OFPT_PORT_STATUS and OFPT_FLOW_REMOVED messages.
. In the \slave" role, enable all OFPT_PORT_STATUS messages and disable all OFPT_PACKET_IN and
OFPT_FLOW_REMOVED messages.
The conguration set with OFPT_SET_ASYNC is specic to a particular OpenFlow channel. It does not
aect any other OpenFlow channel, whether currently established or to be established in the future.
The conguration set with OFPT_SET_ASYNC does not lter or otherwise aect error messages.
91 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
A.4 Asynchronous Messages
A.4.1 Packet-In Message
When packets are received by the datapath and sent to the controller, they use the OFPT_PACKET_IN
message:
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
The buffer_id is an opaque value used by the datapath to identify a buered packet. When a packet
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
The reason eld can be any of these values:
/* Why is this packet being sent to the controller? */
enum ofp_packet_in_reason {
OFPR_NO_MATCH = 0, /* No matching flow (table-miss flow entry). */
OFPR_ACTION = 1, /* Action explicitly output to controller. */
OFPR_INVALID_TTL = 2, /* Packet has invalid TTL */
};
92 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
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
processing.
A.4.2 Flow Removed Message
If the controller has requested to be notied when 
ow entries time out or are deleted from tables, the
datapath does this with the OFPT_FLOW_REMOVED message:
/* Flow removed (datapath -> controller). */
struct ofp_flow_removed {
struct ofp_header header;
uint64_t cookie; /* Opaque controller-issued identifier. */
uint16_t priority; /* Priority level of flow entry. */
uint8_t reason; /* One of OFPRR_*. */
uint8_t table_id; /* ID of the table */
uint32_t duration_sec; /* Time flow was alive in seconds. */
93 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
uint32_t duration_nsec; /* Time flow was alive in nanoseconds beyond
duration_sec. */
uint16_t idle_timeout; /* Idle timeout from original flow mod. */
uint16_t hard_timeout; /* Hard timeout from original flow mod. */
uint64_t packet_count;
uint64_t byte_count;
struct ofp_match match; /* Description of fields. Variable size. */
};
OFP_ASSERT(sizeof(struct ofp_flow_removed) == 56);
The match, cookie, and priority elds are the same as those used in the 
ow mod request.
The reason eld is one of the following:
/* Why was this flow removed? */
enum ofp_flow_removed_reason {
OFPRR_IDLE_TIMEOUT = 0, /* Flow idle time exceeded idle_timeout. */
OFPRR_HARD_TIMEOUT = 1, /* Time exceeded hard_timeout. */
OFPRR_DELETE = 2, /* Evicted by a DELETE flow mod. */
OFPRR_GROUP_DELETE = 3, /* Group was removed. */
};
The duration_sec and duration_nsec elds are described in Section A.3.5.2.
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
they are unsigned and should be set to the maximum eld value if not available.
A.4.3 Port Status Message
As ports are added, modied, and removed from the datapath, the controller needs to be informed with
the OFPT_PORT_STATUS message:
/* A physical port has changed in the datapath */
struct ofp_port_status {
struct ofp_header header;
uint8_t reason; /* One of OFPPR_*. */
uint8_t pad[7]; /* Align to 64-bits. */
struct ofp_port desc;
};
OFP_ASSERT(sizeof(struct ofp_port_status) == 80);
The reason can be one of the following values:
94 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
/* What changed about the physical port */
enum ofp_port_reason {
OFPPR_ADD = 0, /* The port was added. */
OFPPR_DELETE = 1, /* The port was removed. */
OFPPR_MODIFY = 2, /* Some attribute of the port has changed. */
};
A.4.4 Error Message
There are times that the switch needs to notify the controller of a problem. This is done with the
OFPT_ERROR_MSG message:
/* OFPT_ERROR: Error message (datapath -> controller). */
struct ofp_error_msg {
struct ofp_header header;
uint16_t type;
uint16_t code;
uint8_t data[0]; /* Variable-length data. Interpreted based
on the type and code. No padding. */
};
OFP_ASSERT(sizeof(struct ofp_error_msg) == 12);
The type value indicates the high-level type of error. The code value is interpreted based on the type.
The data is variable length and interpreted based on the type and code. Unless specied otherwise, the
data eld contains at least 64 bytes of the failed request that caused the error message to be generated,
if the failed request is shorter than 64 bytes it should be the full request without any padding.
If the error message is in response to a specic message from the controller, e.g., OFPET_BAD_REQUEST,
OFPET_BAD_ACTION, OFPET_BAD_INSTRUCTION, OFPET_BAD_MATCH, or OFPET_FLOW_MOD_FAILED, then
the xid eld of the header must match that of the oending message.
Error codes ending in _EPERM correspond to a permissions error generated by, for example, an OpenFlow
hypervisor interposing between a controller and switch.
Currently dened error types are:
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
95 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
OFPET_ROLE_REQUEST_FAILED = 11, /* Controller Role request failed. */
OFPET_METER_MOD_FAILED = 12, /* Error in meter. */
OFPET_TABLE_FEATURES_FAILED = 13, /* Setting table features failed. */
OFPET_EXPERIMENTER = 0xffff /* Experimenter error messages. */
};
For the OFPET_HELLO_FAILED error type, the following codes are currently dened:
/* ofp_error_msg 'code' values for OFPET_HELLO_FAILED. 'data' contains an
* ASCII text string that may give failure details. */
enum ofp_hello_failed_code {
OFPHFC_INCOMPATIBLE = 0, /* No compatible version. */
OFPHFC_EPERM = 1, /* Permissions error. */
};
The data eld contains an ASCII text string that adds detail on why the error occurred.
For the OFPET_BAD_REQUEST error type, the following codes are currently dened:
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
For the OFPET_BAD_ACTION error type, the following codes are currently dened:
/* ofp_error_msg 'code' values for OFPET_BAD_ACTION. 'data' contains at least
* the first 64 bytes of the failed request. */
enum ofp_bad_action_code {
OFPBAC_BAD_TYPE = 0, /* Unknown action type. */
OFPBAC_BAD_LEN = 1, /* Length problem in actions. */
OFPBAC_BAD_EXPERIMENTER = 2, /* Unknown experimenter id specified. */
OFPBAC_BAD_EXP_TYPE = 3, /* Unknown action for experimenter id. */
OFPBAC_BAD_OUT_PORT = 4, /* Problem validating output port. */
96 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
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
For the OFPET_BAD_INSTRUCTION error type, the following codes are currently dened:
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
For the OFPET_BAD_MATCH error type, the following codes are currently dened:
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
For the OFPET_FLOW_MOD_FAILED error type, the following codes are currently dened:
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
For the OFPET_GROUP_MOD_FAILED error type, the following codes are currently dened:
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
98 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
For the OFPET_PORT_MOD_FAILED error type, the following codes are currently dened:
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
For the OFPET_TABLE_MOD_FAILED error type, the following codes are currently dened:
/* ofp_error_msg 'code' values for OFPET_TABLE_MOD_FAILED. 'data' contains
* at least the first 64 bytes of the failed request. */
enum ofp_table_mod_failed_code {
OFPTMFC_BAD_TABLE = 0, /* Specified table does not exist. */
OFPTMFC_BAD_CONFIG = 1, /* Specified config is invalid. */
OFPTMFC_EPERM = 2, /* Permissions error. */
};
For the OFPET_QUEUE_OP_FAILED error type, the following codes are currently dened:
/* ofp_error msg 'code' values for OFPET_QUEUE_OP_FAILED. 'data' contains
* at least the first 64 bytes of the failed request */
enum ofp_queue_op_failed_code {
OFPQOFC_BAD_PORT = 0, /* Invalid port (or port does not exist). */
OFPQOFC_BAD_QUEUE = 1, /* Queue does not exist. */
OFPQOFC_EPERM = 2, /* Permissions error. */
};
For the OFPET_SWITCH_CONFIG_FAILED error type, the following codes are currently dened:
/* ofp_error_msg 'code' values for OFPET_SWITCH_CONFIG_FAILED. 'data' contains
* at least the first 64 bytes of the failed request. */
enum ofp_switch_config_failed_code {
OFPSCFC_BAD_FLAGS = 0, /* Specified flags is invalid. */
OFPSCFC_BAD_LEN = 1, /* Specified len is invalid. */
OFPSCFC_EPERM = 2, /* Permissions error. */
};
For the OFPET_ROLE_REQUEST_FAILED error type, the following codes are currently dened:
/* ofp_error_msg 'code' values for OFPET_ROLE_REQUEST_FAILED. 'data' contains
* at least the first 64 bytes of the failed request. */
enum ofp_role_request_failed_code {
OFPRRFC_STALE = 0, /* Stale Message: old generation_id. */
OFPRRFC_UNSUP = 1, /* Controller role change unsupported. */
OFPRRFC_BAD_ROLE = 2, /* Invalid role. */
};
99 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
For the OFPET_METER_MOD_FAILED error type, the following codes are currently dened:
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
For the OFPET_TABLE_FEATURES_FAILED error type, the following codes are currently dened:
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
For the OFPET_EXPERIMENTER error type, the error message is dened by the following structure and
elds, followed by experimenter dened data:
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
The experimenter eld is the Experimenter ID, which takes the same form as in struct
ofp_experimenter (see A.5.4).
100 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
A.5 Symmetric Messages
A.5.1 Hello
The OFPT_HELLO message consists of an OpenFlow header plus a set of variable size hello elements.
/* OFPT_HELLO. This message includes zero or more hello elements having
* variable size. Unknown elements types must be ignored/skipped, to allow
* for future extensions. */
struct ofp_hello {
struct ofp_header header;
/* Hello element list */
struct ofp_hello_elem_header elements[0];
};
OFP_ASSERT(sizeof(struct ofp_hello) == 8);
The version eld part of the header eld (see A.1) must be set to the highest OpenFlow protocol
version supported by the sender (see 6.3.1).
The elements eld is a set of hello elements, containing optional data to inform the initial handshake
of the connection. Implementations must ignore (skip) all elements of a Hello message that they do not
support. The list of hello elements types that are currently dened are:
/* Hello elements types.
*/
enum ofp_hello_elem_type {
OFPHET_VERSIONBITMAP = 1, /* Bitmap of version supported. */
};
An element denition contains the element type, length, and any associated data:
/* Common header for all Hello Elements */
struct ofp_hello_elem_header {
uint16_t type; /* One of OFPHET_*. */
uint16_t length; /* Length in bytes of this element. */
};
OFP_ASSERT(sizeof(struct ofp_hello_elem_header) == 4);
The OFPHET_VERSIONBITMAP element use the following structure and elds:
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
101 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
The bitmaps eld indicates the set of versions of the OpenFlow switch protocol a device supports,
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
userspace process, as well as providing more accurate end-to-end latency timing.
A.5.4 Experimenter
The Experimenter message is dened as follows:
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
The experimenter eld is a 32-bit value that uniquely identies the experimenter. If the most signicant
byte is zero, the next three bytes are the experimenter's IEEE OUI. If the most signicant byte is not
zero, it is a value allocated by the Open Networking Foundation. If experimenter does not have (or wish
to use) their OUI, they should contact the Open Networking Foundation to obtain a unique experimenter
ID.
102 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
The rest of the body is uninterpreted by standard OpenFlow processing and is arbitrarily dened by
the corresponding experimenter.
If a switch does not understand a experimenter extension, it must send an OFPT_ERROR message with a
OFPBRC_BAD_EXPERIMENTER error code and OFPET_BAD_REQUEST error type.
Appendix B Release Notes
This section contains release notes highlighting the main changes between the main versions of the
OpenFlow protocol.
The text of the release notes is informative and historical, and should not be considred normative.
Many items of the release notes refer to features and text that has been removed, replaced or updated
in subsequent versions of this specication, and therefore does not necessarily match the actual
specication.
B.1 OpenFlow version 0.2.0
Release date : March 28,2008
Wire Protocol : 1
B.2 OpenFlow version 0.2.1
Release date : March 28,2008
Wire Protocol : 1
No protocol change.
B.3 OpenFlow version 0.8.0
Release date : May 5, 2008
Wire Protocol : 0x83
. Reorganise OpenFlow message types
. Add OFPP_TABLE virtual port to send packet-out packet to the tables
. Add global 
ag OFPC_SEND_FLOW_EXP to congure 
ow expired messages generation
. Add 
ow priority
. Remove 
ow Group-ID (experimental QoS support)
. Add Error messages
. Make stat request and stat reply more generic, with a generic header and stat specic body
. Change fragmentation strategy for stats reply, use explicit 
ag OFPSF_REPLY_MORE instead of
empty packet
103 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
. Add table stats and port stats mesages
B.4 OpenFlow version 0.8.1
Release date : May 20, 2008
Wire Protocol : 0x83
No protocol change.
B.5 OpenFlow version 0.8.2
Release date : October 17, 2008
Wire Protocol : 0x85
. Add Echo Request and Echo Reply messages
. Make all message 64 bits aligned
B.6 OpenFlow version 0.8.9
Release date : December 2, 2008
Wire Protocol : 0x97
B.6.1 IP Netmasks
It is now possible for 
ow entries to contain IP subnet masks. This is done by changes to the wildcards
eld, which has been expanded to 32-bits:
/* Flow wildcards. */
enum ofp_flow_wildcards {
OFPFW_IN_PORT = 1 << 0, /* Switch input port. */
OFPFW_DL_VLAN = 1 << 1, /* VLAN. */
OFPFW_DL_SRC = 1 << 2, /* Ethernet source address. */
OFPFW_DL_DST = 1 << 3, /* Ethernet destination address. */
OFPFW_DL_TYPE = 1 << 4, /* Ethernet frame type. */
OFPFW_NW_PROTO = 1 << 5, /* IP protocol. */
OFPFW_TP_SRC = 1 << 6, /* TCP/UDP source port. */
OFPFW_TP_DST = 1 << 7, /* TCP/UDP destination port. */
/* IP source address wildcard bit count. 0 is exact match, 1 ignores the
* LSB, 2 ignores the 2 least-significant bits, ..., 32 and higher wildcard
* the entire field. This is the *opposite* of the usual convention where
* e.g. /24 indicates that 8 bits (not 24 bits) are wildcarded. */
OFPFW_NW_SRC_SHIFT = 8,
OFPFW_NW_SRC_BITS = 6,
OFPFW_NW_SRC_MASK = ((1 << OFPFW_NW_SRC_BITS) - 1) << OFPFW_NW_SRC_SHIFT,
OFPFW_NW_SRC_ALL = 32 << OFPFW_NW_SRC_SHIFT,
/* IP destination address wildcard bit count. Same format as source. */
OFPFW_NW_DST_SHIFT = 14,
104 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
OFPFW_NW_DST_BITS = 6,
OFPFW_NW_DST_MASK = ((1 << OFPFW_NW_DST_BITS) - 1) << OFPFW_NW_DST_SHIFT,
OFPFW_NW_DST_ALL = 32 << OFPFW_NW_DST_SHIFT,
/* Wildcard all fields. */
OFPFW_ALL = ((1 << 20) - 1)
};
The source and destination netmasks are each specied with a 6-bit number in the wildcard description.
It is interpreted similar to the CIDR sux, but with the opposite meaning, since this is being used to
indicate which bits in the IP address should be treated as "wild". For example, a CIDR sux of "24"
means to use a netmask of "255.255.255.0". However, a wildcard mask value of "24" means that the
least-signicant 24-bits are wild, so it forms a netmask of "255.0.0.0".
B.6.2 New Physical Port Stats
The ofp_port_stats message has been expanded to return more information. If a switch does not
support a particular eld, it should set the value to have all bits enabled (i.e., a "-1" if the value were
treated as signed). This is the new format:
/* Body of reply to OFPST_PORT request. If a counter is unsupported, set
* the field to all ones. */
struct ofp_port_stats {
uint16_t port_no;
uint8_t pad[6]; /* Align to 64-bits. */
uint64_t rx_packets; /* Number of received packets. */
uint64_t tx_packets; /* Number of transmitted packets. */
uint64_t rx_bytes; /* Number of received bytes. */
uint64_t tx_bytes; /* Number of transmitted bytes. */
uint64_t rx_dropped; /* Number of packets dropped by RX. */
uint64_t tx_dropped; /* Number of packets dropped by TX. */
uint64_t rx_errors; /* Number of receive errors. This is a super-set
of receive errors and should be great than or
equal to the sum of al rx_*_err values. */
uint64_t tx_errors; /* Number of transmit errors. This is a super-set
of transmit errors. */
uint64_t rx_frame_err; /* Number of frame alignment errors. */
uint64_t rx_over_err; /* Number of packets with RX overrun. */
uint64_t rx_crc_err; /* Number of CRC errors. */
uint64_t collisions; /* Number of collisions. */
};
B.6.3 IN PORT Virtual Port
The behavior of sending out the incoming port was not clearly dened in earlier versions of the speci
cation. It is now forbidden unless the output port is explicitly set to OFPP_IN_PORT virtual port
(0xf8) is set. The primary place where this is used is for wireless links, where a packet is received over
the wireless interface and needs to be sent to another host through the same interface. For example,
if a packet needed to be sent to all interfaces on the switch, two actions would need to be specied:
"actions=output:ALL,output:IN PORT".
105 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
B.6.4 Port and Link Status and Conguration
The switch should inform the controller of changes to port and link status. This is done with a new

ag in ofp_port_config:
. OFPPC_PORT_DOWN - The port has been congured "down".
... and a new 
ag in ofp_port_state:
. OFPPS_LINK_DOWN - There is no physical link present.
The switch should support enabling and disabling a physical port by modifying the OFPPFL_PORT_DOWN

ag (and mask bit) in the ofp_port_mod message. Note that this is not the same as adding or removing
the interface from the list of OpenFlow monitored ports; it is equivalent to "ifconfig eth0 down" on
Unix systems.
B.6.5 Echo Request/Reply Messages
The switch and controller can verify proper connectivity through the OpenFlow protocol with the new
echo request (OFPT_ECHO_REQUEST) and reply (OFPT_ECHO_REPLY) messages. The body of the message
is undened and simply contains uninterpreted data that is to be echoed back to the requester. The
requester matches the reply with the transaction id from the OpenFlow header.
B.6.6 Vendor Extensions
Vendors are now able to add their own extensions, while still being OpenFlow compliant. The primary
way to do this is with the new OFPT_VENDOR message type. The message body is of the form:
/* Vendor extension. */
struct ofp_vendor {
struct ofp_header header; /* Type OFPT_VENDOR. */
uint32_t vendor; /* Vendor ID:
* - MSB 0: low-order bytes are IEEE OUI.
* - MSB != 0: defined by OpenFlow
* consortium. */
/* Vendor-defined arbitrary additional data. */
};
The vendor eld is a 32-bit value that uniquely identies the vendor. If the most signicant byte is zero,
the next three bytes are the vendor's IEEE OUI. If vendor does not have (or wish to use) their OUI,
they should contact the OpenFlow consortium to obtain one. The rest of the body is uninterpreted.
It is also possible to add vendor extensions for stats messages with the OFPST_VENDOR stats type. The
rst four bytes of the message are the vendor identier as described earlier. The rest of the body is
vendor-dened.
To indicate that a switch does not understand a vendor extension, a OFPBRC_BAD_VENDOR error code
has been dened under the OFPET_BAD_REQUEST error type.
Vendor-dened actions are described below in the "Variable Length and Vendor Actions" section.
106 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
B.6.7 Explicit Handling of IP Fragments
In previous versions of the specication, handling of IP fragments was not clearly dened. The switch
is now able to tell the controller whether it is able to reassemble fragments. This is done with the
following capabilities 
ag passed in the ofp_switch features message:
OFPC_IP_REASM = 1 << 5 /* Can reassemble IP fragments. */
The controller can congure fragment handling in the switch through the setting the following new
ofp_config_flags in the ofp_switch_config message:
/* Handling of IP fragments. */
OFPC_FRAG_NORMAL = 0 << 1, /* No special handling for fragments. */
OFPC_FRAG_DROP = 1 << 1, /* Drop fragments. */
OFPC_FRAG_REASM = 2 << 1, /* Reassemble (only if OFPC_IP_REASM set). */
OFPC_FRAG_MASK = 3 << 1
"Normal" handling of fragments means that an attempt should be made to pass the fragments through
the OpenFlow tables. If any eld is not present (e.g., the TCP/UDP ports didn't t), then the packet
should not match any entry that has that eld set.
B.6.8 802.1D Spanning Tree
OpenFlow now has a way to congure and view results of on-switch implementations of 802.1D Spanning
Tree Protocol.
A switch that implements STP must set the new OFPC_STP bit in the 'capabilities' eld of its
OFPT_FEATURES_REPLY message. A switch that implements STP at all must make it available on all of
its physical ports, but it need not implement it on virtual ports (e.g. OFPP_LOCAL).
Several port conguration 
ags are associated with STP. The complete set of port conguration 
ags
are:
enum ofp_port_config {
OFPPC_PORT_DOWN = 1 << 0, /* Port is administratively down. */
OFPPC_NO_STP = 1 << 1, /* Disable 802.1D spanning tree on port. */
OFPPC_NO_RECV = 1 << 2, /* Drop most packets received on port. */
OFPPC_NO_RECV_STP = 1 << 3, /* Drop received 802.1D STP packets. */
OFPPC_NO_FLOOD = 1 << 4, /* Do not include this port when flooding. */
OFPPC_NO_FWD = 1 << 5, /* Drop packets forwarded to port. */
OFPPC_NO_PACKET_IN = 1 << 6 /* Do not send packet-in msgs for port. */
};
The controller may set OFPPFL_NO_STP to 0 to enable STP on a port or to 1 to disable STP on a port.
(The latter corresponds to the Disabled STP port state.) The default is switch implementation-dened;
the OpenFlow reference implementation by default sets this bit to 0 (enabling STP).
When OFPPFL_NO_STP is 0, STP controls the OFPPFL_NO_FLOOD and OFPPFL_STP_* bits directly.
OFPPFL_NO_FLOOD is set to 0 when the STP port state is Forwarding, otherwise to 1. The bits in
107 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
OFPPFL_STP_MASK are set to one of the other OFPPFL_STP_* values according to the current STP port
state.
When the port 
ags are changed by STP, the switch sends an OFPT_PORT_STATUS message to notify
the controller of the change. The OFPPFL_NO_RECV, OFPPFL_NO_RECV_STP, OFPPFL_NO_FWD, and
OFPPFL_NO_PACKET_IN bits in the OpenFlow port 
ags may be useful for the controller to implement
STP, although they interact poorly with in-band control.
B.6.9 Modify Actions in Existing Flow Entries
New ofp_flow_mod commands have been added to support modifying the actions of existing entries:
OFPFC_MODIFY and OFPFC_MODIFY_STRICT. They use the match eld to describe the entries that should
be modied with the supplied actions. OFPFC_MODIFY is similar to OFPFC_DELETE, in that wildcards
are "active". OFPFC_MODIFY_STRICT is similar to OFPFC_DELETE_STRICT, in that wildcards are not
"active", so both the wildcards and priority must match an entry. When a matching 
ow is found, only
its actions are modied{information such as counters and timers are not reset.
If the controller uses the OFPFC_ADD command to add an entry that already exists, then the new entry
replaces the old and all counters and timers are reset.
B.6.10 More Flexible Description of Tables
Previous versions of OpenFlow had very limited abilities to describe the tables supported by the switch.
The n_exact, n_compression, and n_general elds in ofp_switch_features have been replaced with
n_tables, which lists the number of tables in the switch.
The behavior of the OFPST_TABLE stat reply has been modied slightly. The ofp_table_stats body now
contains a wildcards eld, which indicates the elds for which that particular table supports wildcarding.
For example, a direct look-up hash table would have that eld set to zero, while a sequentially searched
table would have it set to OFPFW_ALL. The ofp_table_stats entries are returned in the order that
packets traverse the tables.
When the controller and switch rst communicate, the controller will nd out how many tables the
switch supports from the Features Reply. If it wishes to understand the size, types, and order in which
tables are consulted, the controller sends a OFPST_TABLE stats request.
B.6.11 Lookup Count in Tables
Table stats returned ofp_table_stats structures now return the number of packets that have been
looked up in the table{whether they hit or not. This is stored in the lookup_count eld.
108 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
B.6.12 Modifying Flags in Port-Mod More Explicit
The ofp_port_mod is used to modify characteristics of a switch's ports. A supplied ofp_phy_port
structure describes the behavior of the switch through its flags eld. However, it's possible that the
controller wishes to change a particular 
ag and may not know the current status of all 
ags. A mask
eld has been added which has a bit set for each 
ag that should be changed on the switch.
The new ofp_port_mod message looks like the following:
/* Modify behavior of the physical port */
struct ofp_port_mod {
struct ofp_header header;
uint32_t mask; /* Bitmap of "ofp_port_flags" that should be
changed. */
struct ofp_phy_port desc;
};
B.6.13 New Packet-Out Message Format
The previous version's packet-out message treated the variable-length array dierently depending on
whether the buffer_id was set or not. If set, the array consisted of actions to be executed and the
out_port was ignored. If not, the array consisted of the actual packet that should be placed on the
wire through the out_port interface. This was a bit ugly, and it meant that in order for a non-buered
packet to have multiple actions executed on it, that a new 
ow entry be created just to match that
entry.
A new format is now used, which cleans the message up a bit. The packet always contains a list of
actions. An additional variable-length array follows the list of actions with the contents of the packet
if buffer_id is not set. This is the new format:
struct ofp_packet_out {
struct ofp_header header;
uint32_t buffer_id; /* ID assigned by datapath (-1 if none). */
uint16_t in_port; /* Packet's input port (OFPP_NONE if none). */
uint16_t n_actions; /* Number of actions. */
struct ofp_action actions[0]; /* Actions. */
/* uint8_t data[0]; */ /* Packet data. The length is inferred
from the length field in the header.
(Only meaningful if buffer_id == -1.) */
};
B.6.14 Hard Timeout for Flow Entries
A hard timeout value has been added to 
ow entries. If set, then the entry must be expired in the
specied number of seconds regardless of whether or not packets are hitting the entry. A hard_timeout
eld has been added to the flow_mod message to support this. The max_idle eld has been renamed
idle_timeout. A value of zero means that a timeout has not been set. If both idle_timeout and
hard_timeout are zero, then the 
ow is permanent and should not be deleted without an explicit
deletion.
109 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
The new ofp_flow_mod format looks like this:
struct ofp_flow_mod {
struct ofp_header header;
struct ofp_match match; /* Fields to match */
/* Flow actions. */
uint16_t command; /* One of OFPFC_*. */
uint16_t idle_timeout; /* Idle time before discarding (seconds). */
uint16_t hard_timeout; /* Max time before discarding (seconds). */
uint16_t priority; /* Priority level of flow entry. */
uint32_t buffer_id; /* Buffered packet to apply to (or -1).
Not meaningful for OFPFC_DELETE*. */
uint32_t reserved; /* Reserved for future use. */
struct ofp_action actions[0]; /* The number of actions is inferred from
the length field in the header. */
};
Since 
ow entries can now be expired due to idle or hard timeouts, a reason eld has been added to the
ofp_flow_expired message. A value of 0 indicates an idle timeout and 1 indicates a hard timeout:
enum ofp_flow_expired_reason {
OFPER_IDLE_TIMEOUT, /* Flow idle time exceeded idle_timeout. */
OFPER_HARD_TIMEOUT /* Time exceeded hard_timeout. */
};
The new ofp_flow_expired message looks like the following:
struct ofp_flow_expired {
struct ofp_header header;
struct ofp_match match; /* Description of fields */
uint16_t priority; /* Priority level of flow entry. */
uint8_t reason; /* One of OFPER_*. */
uint8_t pad[1]; /* Align to 32-bits. */
uint32_t duration; /* Time flow was alive in seconds. */
uint8_t pad2[4]; /* Align to 64-bits. */
uint64_t packet_count;
uint64_t byte_count;
};
B.6.15 Reworked initial handshake to support backwards compatibility
OpenFlow now includes a basic "version negotiation" capability. When an OpenFlow connection is
established, each side of the connection should immediately send an OFPT_HELLO message as its rst
OpenFlow message. The 'version' eld in the hello message should be the highest OpenFlow protocol
version supported by the sender. Upon receipt of this message, the recipient may calculate the OpenFlow
protocol version to be used as the smaller of the version number that it sent and the one that it
received.
110 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
If the negotiated version is supported by the recipient, then the connection proceeds. Otherwise, the
recipient must reply with a message of OFPT_ERROR with a 'type' value of OFPET_HELLO_FAILED, a
'code' of OFPHFC_COMPATIBLE, and optionally an ASCII string explaining the situation in 'data', and
then terminate the connection.
The OFPT_HELLO message has no body; that is, it consists only of an OpenFlow header. Implementations
must be prepared to receive a hello message that includes a body, ignoring its contents, to allow for
later extensions.
B.6.16 Description of Switch Stat
The OFPST_DESC stat has been added to describe the hardware and software running on the switch:
#define DESC_STR_LEN 256
#define SERIAL_NUM_LEN 32
/* Body of reply to OFPST_DESC request. Each entry is a NULL-terminated
* ASCII string. */
struct ofp_desc_stats {
char mfr_desc[DESC_STR_LEN]; /* Manufacturer description. */
char hw_desc[DESC_STR_LEN]; /* Hardware description. */
char sw_desc[DESC_STR_LEN]; /* Software description. */
char serial_num[SERIAL_NUM_LEN]; /* Serial number. */
};
It contains a 256 character ASCII description of the manufacturer, hardware type, and software version.
It also contains a 32 character ASCII serial number. Each entry is padded on the right with 0 bytes.
B.6.17 Variable Length and Vendor Actions
Vendor-dened actions have been added to OpenFlow. To enable more versatility, actions have switched
from xed-length to variable. All actions have the following header:
struct ofp_action_header {
uint16_t type; /* One of OFPAT_*. */
uint16_t len; /* Length of action, including this
header. This is the length of action,
including any padding to make it
64-bit aligned. */
uint8_t pad[4];
};
The length for actions must always be a multiple of eight to aid in 64-bit alignment. The action types
are as follows:
111 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
enum ofp_action_type {
OFPAT_OUTPUT, /* Output to switch port. */
OFPAT_SET_VLAN_VID, /* Set the 802.1q VLAN id. */
OFPAT_SET_VLAN_PCP, /* Set the 802.1q priority. */
OFPAT_STRIP_VLAN, /* Strip the 802.1q header. */
OFPAT_SET_DL_SRC, /* Ethernet source address. */
OFPAT_SET_DL_DST, /* Ethernet destination address. */
OFPAT_SET_NW_SRC, /* IP source address. */
OFPAT_SET_NW_DST, /* IP destination address. */
OFPAT_SET_TP_SRC, /* TCP/UDP source port. */
OFPAT_SET_TP_DST, /* TCP/UDP destination port. */
OFPAT_VENDOR = 0xffff
};
The vendor-dened action header looks like the following:
struct ofp_action_vendor_header {
uint16_t type; /* OFPAT_VENDOR. */
uint16_t len; /* Length is 8. */
uint32_t vendor; /* Vendor ID, which takes the same form
as in "struct ofp_vendor". */
};
The vendor eld uses the same vendor identier described earlier in the "Vendor Extensions" section.
Beyond using the ofp_action_vendor header and the 64-bit alignment requirement, vendors are free
to use whatever body for the message they like.
B.6.18 VLAN Action Changes
It is now possible to set the priority eld in VLAN tags and stripping VLAN tags is now a separate
action. The OFPAT_SET_VLAN_VID action behaves like the former OFPAT_SET_DL_VLAN action, but no
longer accepts a special value that causes it to strip the VLAN tag. The OFPAT_SET_VLAN_PCP action
modies the 3-bit priority eld in the VLAN tag. For existing tags, both actions only modify the bits
associated with the eld being updated. If a new VLAN tag needs to be added, the value of all other
elds is zero.
The OFPAT_SET_VLAN_VID action looks like the following:
struct ofp_action_vlan_vid {
uint16_t type; /* OFPAT_SET_VLAN_VID. */
uint16_t len; /* Length is 8. */
uint16_t vlan_vid; /* VLAN id. */
uint8_t pad[2];
};
The OFPAT_SET_VLAN_PCP action looks like the following:
112 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
struct ofp_action_vlan_pcp {
uint16_t type; /* OFPAT_SET_VLAN_PCP. */
uint16_t len; /* Length is 8. */
uint8_t vlan_pcp; /* VLAN priority. */
uint8_t pad[3];
};
The OFPAT_STRIP_VLAN action takes no argument and strips the VLAN tag if one is present.
B.6.19 Max Supported Ports Set to 65280
What: Increase maximum number of ports to support large vendor switches; was previously 256, chosen
arbitrarily.
Why: The HP 5412 chassis supports 288 ports of Ethernet, and some Cisco switches go much higher.
The current limit (OFPP_MAX) is 255, set to equal the maximum number of ports in a bridge segment in
the 1998 STP spec. The RSTP spec from 2004 supports up to 4096 (12 bits) of ports.
How: Change OFPP_MAX to 65280. (However, out of the box, the reference switch implementation
supports at most 256 ports.)
B.6.20 Send Error Message When Flow Not Added Due To Full Tables
The switch now sends an error message when a 
ow is added, but cannot because all the tables are full.
The message has an error type of OFPET_FLOW_MOD_FAILED and code of OFPFMFC_ALL_TABLES_FULL. If
the Flow-Mod command references a buered packet, then actions are not performed on the packet. If
the controller wishes the packet to be sent regardless of whether or not a 
ow entry is added, then it
should use a Packet-Out directly.
B.6.21 Behavior Dened When Controller Connection Lost
What: Ensure that all switches have at least one common behavior when the controller connection is
lost.
Why: When the connection to the controller is lost, the switch should behave in a well-dened way.
Reasonable behaviors include 'do nothing - let 
ows naturally timeout', 'freeze timeouts', 'become
learning switch', and 'attempt connection to other controller'. Switches may implement one or more
of these, and network admins may want to ensure that if the controller goes out, they know what the
network can do.
The rst is the simplest: ensure that every switch implements a default of 'do nothing - let 
ows timeout
naturally'. Changes must be done via vendor-specic command line interface or vendor extension
OpenFlow messages.
The second may help ensure that a single controller can work with switches from multiple vendors. The
dierent failure behaviors, plus 'other', could be feature bits set for the switch. A switch would still
only have to support the default.
113 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
The worry here is that we may not be able to enumerate in advance the full range of failure behaviors,
which argues for the rst approach.
How: Added text to spec: "In the case that the switch loses contact with the controller, the default
behavior must be to do nothing - to let 
ows timeout naturally. Other behaviors can be implemented
via vendor-specic command line interface or vendor extension OpenFlow messages."
B.6.22 ICMP Type and Code Fields Now Matchable
What: Allow matching ICMP trac based on type or code.
Why: We can't distinguish between dierent types of ICMP trac (e.g., echo replies vs echo requests
vs redirects).
How: Changed spec to allow matching on these elds.
As for implementation: The type and code are each a single byte, so they easily t in our existing 
ow
structure. Overload the tp_src eld to ICMP type and tp_dst to ICMP code. Since they are only a
single byte, they will reside in the low-byte of these two byte elds (stored in network-byte order). This
will allow a controller to use the existing wildcard bits to wildcard these ICMP elds.
B.6.23 Output Port Filtering for Delete*, Flow Stats and Aggregate Stats
Add support for listing and deleting entries based on an output port.
To support this, an out_port eld has been added to the ofp_flow_mod, ofp_flow_stats_request,
and ofp_aggregate_stats_request messages. If an out_port contains a value other than OFPP_NONE,
it introduces a constraint when matching. This constraint is that the rule must contain an output action
directed at that port. Other constraints such as ofp_match structs and priorities are still used; this is
purely an *additional* constraint. Note that to get previous behavior, though, out_port must be set
to OFPP_NONE, since "0" is a valid port id. This only applies to the delete and delete_strict 
ow
mod commands; the eld is ignored by add, modify, and modify_strict.
B.7 OpenFlow version 0.9
Release date : July 20, 2009
Wire Protocol : 0x98
B.7.1 Failover
The reference implementation now includes a simple failover mechanism. A switch can be congured
with a list of controllers. If the rst controller fails, it will automatically switch over to the second
controller on the list.
114 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
B.7.2 Emergency Flow Cache
The protocol and reference implementation have been extended to allow insertion and management of
emergency 
ow entries.
Emergency-specic 
ow entries are inactive until a switch loses connectivity from the controller. If this
happens, the switch invalidates all normal 
ow table entries and copies all emergency 
ows into the
normal 
ow table.
Upon connecting to a controller again, all entries in the 
ow cache stay active. The controller then has
the option of resetting the 
ow cache if needed.
B.7.3 Barrier Command
The Barrier Command is a mechanism to get notied when an OpenFlow message has nished executing
on the switch. When a switch receives a Barrier message it must rst complete all commands sent before
the Barrier message before executing any commands after it. When all commands before the Barrier
message have completed, it must send a Barrier Reply message back to the controller.
B.7.4 Match on VLAN Priority Bits
There is an optional new feature that allows matching on priority VLAN elds. Pre 0.9, the VLAN id
is a eld used in identifying a 
ow, but the priority bits in the VLAN tag are not. In this release we
include the priority bits as a separate eld to identify 
ows. Matching is possible as either an exact
match on the 3 priority bits, or as a wildcard for the entire 3 bits.
B.7.5 Selective Flow Expirations
Flow expiration messages can now be requested on a per-
ow, rather than per-switch granularity.
B.7.6 Flow Mod Behavior
There now is a CHECK_OVERLAP 
ag to 
ow mods which requires the switch to do the (potentially more
costly) check that there doesn't already exist a con
icting 
ow with the same priority. If there is one,
the mod fails and an error code is returned. Support for this 
ag is required in an OpenFlow switch.
B.7.7 Flow Expiration Duration
The meaning of the "duration" eld in the Flow Expiration message has been changed slightly. Previously
there were con
icting denitions of this in the spec. In 0.9 the value returned will be the time
that the 
ow was active and not include the timeout period.
115 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
B.7.8 Notication for Flow Deletes
If a controller deletes a 
ow it now receives a notication if the notication bit is set. In previous
releases only 
ow expirations but not delete actions would trigger notications.
B.7.9 Rewrite DSCP in IP ToS header
There is now an added Flow action to rewrite the DiServ CodePoint bits part of the IP ToS eld in
the IP header. This enables basic support for basic QoS with OpenFlow in some switches. A more
complete QoS framework is planned for a future OpenFlow release.
B.7.10 Port Enumeration now starts at 1
Previous releases of OpenFlow had port numbers start at 0, release 0.9 changes them to start at 1.
B.7.11 Other changes to the Specication
. 6633/TCP is now the recommended default OpenFlow Port. Long term the goal is to get a IANA
approved port for OpenFlow.
. The use of "Type 1" and "Type 0" has been depreciated and references to it have been removed.
. Claried Matching Behavior for Flow Modication and Stats
. Made explicit that packets received on ports that are disabled by spanning tree must follow the
normal 
ow table processing path.
. Claried that transaction ID in header should match oending message for OFPET_BAD_REQUEST,
OFPET_BAD_ACTION, OFPET_FLOW_MOD_FAILED.
. Claried the format for the Strip VLAN Action
. Clarify behavior for packets that are buered on the switch while switch is waiting for a reply
from controller
. Added the new EPERM Error Type
. Fixed Flow Table Matching Diagram
. Claried datapath ID 64 bits, up from 48 bits
. Claried miss-send-len and max-len of output action
B.8 OpenFlow version 1.0
Release date : December 31, 2009
Wire Protocol : 0x01
B.8.1 Slicing
OpenFlow now supports multiple queues per output port. Queues support the ability to provide minimum
bandwidth guarantees; the bandwidth allocated to each queue is congurable. The name slicing
is derived from the ability to provide a slice of the available network bandwidth to each queue.
116 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
B.8.2 Flow cookies
Flows have been extended to include an opaque identier, referred to as a cookie. The cookie is specied
by the controller when the 
ow is installed; the cookie will be returned as part of each 
ow stats and

ow expired message.
B.8.3 User-speciable datapath description
The OFPST DESC (switch description) reply now includes a datapath description eld. This is a userspeci
able eld that allows a switch to return a string specied by the switch owner to describe the
switch.
B.8.4 Match on IP elds in ARP packets
The reference implementation can now match on IP elds inside ARP packets. The source and destination
protocol address are mapped to the nw src and nw dst elds respecitively, and the opcode is
mapped to the nw proto eld.
B.8.5 Match on IP ToS/DSCP bits
OpenFlow now supports matching on the IP ToS/DSCP bits.
B.8.6 Querying port stats for individual ports
Port stat request messages include a port_no eld to allow stats for individual ports to be queried.
Port stats for all ports can still be requested by specifying OFPP_NONE as the port number.
B.8.7 Improved 
ow duration resolution in stats/expiry messages
Flow durations in stats and expiry messages are now expressed with nanosecond resolution. Note that
the accuracy of 
ow durations in the reference implementation is on the order of milliseconds. (The
actual accuracy is in part dependent upon kernel parameters.)
B.8.8 Other changes to the Specication
. remove multi_phy_tx spec text and capability bit
. clarify execution order of actions
. replace SSL refs with TLS
. resolve overlap ambiguity
. clarify 
ow mod to non-existing port
. clarify port denition
. update packet 
ow diagram
. update header parsing diagram for ICMP
117 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
. x English ambiguity for 
ow-removed messages
. x async message English ambiguity
. note that multiple controller support is undened
. clarify that byte equals octet
. note counter wrap-around
. removed warning not to build a switch from this specication
B.9 OpenFlow version 1.1
Release date : February 28, 2011
Wire Protocol : 0x02
B.9.1 Multiple Tables
Prior versions of the OpenFlow specication did expose to the controller the abstraction of a single
table. The OpenFlow pipeline could internally be mapped to multiple tables, such as having a separate
wildcard and exact match table, but those tables would always act logically as a single table.
OpenFlow 1.1 introduces a more 
exible pipeline with multiple tables. Exposing multiple tables has
many advantages. The rst advantage is that many hardware have multiple tables internally (for
example L2 table, L3 table, multiple TCAM lookups), and the multiple table support of OpenFlow
may enable to expose this hardware with greater eciency and 
exibility. The second advantage is
that many network deployments combine orthogonal processing of packets (for example ACL, QoS and
routing), forcing all those processing in a single table creates huge ruleset due to the cross product of
individual rules, multiple tables may decouple properly those processing.
The new OpenFlow pipeline with multiple table is quite dierent from the simple pipeline of prior
OpenFlow versions. The new OpenFlow pipeline expose a set of completely generic tables, supporting
the full match and full set of actions. It's dicult to build a pipeline abstraction that represent accurately
all possible hardware, therefore OpenFlow 1.1 is based on a generic and 
exible pipeline that may be
mapped to the hardware. Some limited table capabilities are available to denote what each table is
capable of supporting.
Packets are processed through the pipeline, they are matched and processed in the rst table, and may
be matched and processed in other tables. As it goes through the pipeline, a packet is associated with
an action set, accumulating action, and a generic metadata register. The action set is resolved at the
end of the pipeline and applied to the packet. The metadata can be matched and written at each table
and enables to carry state between tables.
OpenFlow introduces a new protocol object called instruction to control pipeline processing. Actions
which were directly attached to 
ows in previous versions are now encapsulated in instructions, instructions
may apply those actions between tables or accumulate them in the packet action set. Instructions
can also change the metadata, or direct packet to another table.
. The switch now expose a pipeline with multiple tables
. Flow entry have instructions to control pipeline processing
. Controller can choose packet traversal of tables via goto instruction
. Metadata eld (64 bits) can be set and match in tables
118 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
. Packet actions can be merged in packet action set
. Packet action set is executed at the end of pipeline
. Packet actions can be applied between table stages
. Table miss can send to controller, continue to next table or drop
. Rudimentary table capability and conguration
B.9.2 Groups
The new group abstraction enables OpenFlow to represent a set of ports as a single entity for forwarding
packets. Dierent types of groups are provided, to represent dierent abstractions such as multicasting
or multipathing. Each group is composed of a set group buckets, each group bucket contains the set of
actions to be applied before forwarding to the port. Groups buckets can also forward to other groups,
enabling to chain groups together.
. Group indirection to represent a set of ports
. Group table with 4 types of groups :
{ All - used for multicast and 
ooding
{ Select - used for multipath
{ Indirect - simple indirection
{ Fast Failover - use rst live port
. Group action to direct a 
ow to a group
. Group buckets contains actions related to the individual port
B.9.3 Tags : MPLS & VLAN
Prior versions of the OpenFlow specication had limited VLAN support, it only supported a single
level of VLAN tagging with ambiguous semantic. The new tagging support has explicit actions to add,
modify and remove VLAN tags, and can support multiple level of VLAN tagging. It also adds similar
support the MPLS shim headers.
. Support for VLAN and QinQ, adding, modifying and removing VLAN headers
. Support for MPLS, adding, modifying and removing MPLS shim headers
B.9.4 Virtual ports
Prior versions of the OpenFlow specication assumed that all the ports of the OpenFlow switch were
physical ports. This version of the specication add support for virtual ports, which can represent
complex forwarding abstractions such as LAGs or tunnels.
. Make port number 32 bits, enable larger number of ports
. Enable switch to provide virtual ports as OpenFlow ports
. Augment packet-in to report both virtual and physical ports
119 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
B.9.5 Controller connection failure
Prior versions of the OpenFlow specication introduced the emergency 
ow cache as a way to deal
with the loss of connectivity with the controller. The emergency 
ow cache feature was removed in this
version of the specication, due to the lack of adoption, the complexity to implement it and other issues
with the feature semantic.
This version of the specication adds two simpler modes to deal with the loss of connectivity with the
controller. In fail secure mode, the switch continues operating in OpenFlow mode, until it reconnects to a
controller. In fail standalone mode, the switch revert to using normal processing (Ethernet switching).
. Remove Emergency Flow Cache from spec
. Connection interruption triggers fail secure or fail standalone mode
B.9.6 Other changes
. Remove 802.1d-specic text from the specication
. Cookie Enhancements Proposal - cookie mask for ltering
. Set queue action (unbundled from output port action)
. Maskable DL and NW address match elds
. Add TTL decrement, set and copy actions for IPv4 and MPLS
. SCTP header matching and rewriting support
. Set ECN action
. Dene message handling : no loss, may reorder if no barrier
. Rename VENDOR APIs to EXPERIMENTER APIs
. Many other bug xes, rewording and clarications
B.10 OpenFlow version 1.2
Release date : December 5, 2011
Wire Protocol : 0x03
Please refers to the bug tracking ID for more details on each change
B.10.1 Extensible match support
Prior versions of the OpenFlow specication used a static xed length structure to specify ofp_match,
which prevents 
exible expression of matches and prevents inclusion of new match elds. The ofp_match
has been changed to a TLV structure, called OpenFlow Extensible Match (OXM), which dramatically
increases 
exibility.
The match elds themselves have been reorganised. In the previous static structure, many elds were
overloaded ; for example tcp.src_port, udp.src_port, and icmp.code were using the same eld entry.
Now, every logical eld has its own unique type.
List of features for OpenFlow Extensible Match :
. Flexible and compact TLV structure called OXM (EXT-1)
120 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
. Enable 
exible expression of match, and 
exible bitmasking (EXT-1)
. Pre-requisite system to insure consistency of match (EXT-1)
. Give every match eld a unique type, remove overloading (EXT-1)
. Modify VLAN matching to be more 
exible (EXT-26)
. Add vendor classes and experimenter matches (EXT-42)
. Allow switches to override match requirements (EXT-56, EXT-33)
B.10.2 Extensible 'set eld' packet rewriting support
Prior versions of the OpenFlow specication were using hand-crafted actions to rewrite header elds.
The Extensible set_field action reuses the OXM encoding dened for matches, and enables to rewrite
any header eld in a single action (EXT-13). This allows any new match eld, including experimenter
elds, to be available for rewrite. This makes the specication cleaner and eases cost of introducing
new elds.
. Deprecate most header rewrite actions
. Introduce generic set-field action (EXT-13)
. Reuse match TLV structure (OXM) in set-field action
B.10.3 Extensible context expression in 'packet-in'
The packet-in message did include some of the packet context (ingress port), but not all (metadata),
preventing the controller from guring how match did happen in the table and which 
ow entries would
match or not match. Rather than introduce a hard coded eld in the packet-in message, the 
exible
OXM encoding is used to carry packet context.
. Reuse match TLV structure (OXM) to describe metadata in packet-in (EXT-6)
. Include the 'metadata' eld in packet-in
. Move ingress port and physical port from static eld to OXM encoding
. Allow to optionally include packet header elds in TLV structure
B.10.4 Extensible Error messages via experimenter error type
An experimenter error code has been added, enabling experimenter functionality to generate custom
error messages (EXT-2). The format is identical to other experimenter APIs.
B.10.5 IPv6 support added
Basic support for IPv6 match and header rewrite has been added, via the Flexible match support.
. Added support for matching on IPv6 source address, destination address, protocol number, trac
class, ICMPv6 type, ICMPv6 code and IPv6 neighbor discovery header elds (EXT-1)
. Added support for matching on IPv6 
ow label (EXT-36)
121 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
B.10.6 Simplied behaviour of 
ow-mod request
The behaviour of 
ow-mod request has been simplied (EXT-30).
. MODIFY and MODIFY STRICT commands never insert new 
ows in the table
. New 
ag OFPFF RESET COUNTS to control counter reset
. Remove quirky behaviour for cookie eld.
B.10.7 Removed packet parsing specication
The OpenFlow specication no longer attempts to dene how to parse packets (EXT-3). The match
elds are only dened logically.
. OpenFlow does not mandate how to parse packets
. Parsing consistency acheived via OXM pre-requisite
B.10.8 Controller role change mechanism
The controller role change mechanism is a simple mechanism to support multiple controllers for failover
(EXT-39). This scheme is entirely driven by the controllers ; the switch only need to remember the role
of each controller to help the controller election mechanism.
. Simple mechanism to support multiple controllers for failover
. Switches may now connect to multiple controllers in parallel
. Enable each controller to change its roles to equal, master or slave
B.10.9 Other changes
. Per-table metadata bitmask capabilities (EXT-34)
. Rudimentary group capabilities (EXT-61)
. Add hard timeout info in 
ow-removed messages (OFP-283)
. Add ability for controller to detect STP support(OFP-285)
. Turn o packet buering with OFPCML NO BUFFER (EXT-45)
. Added ability to query all queues (EXT-15)
. Added experimenter queue property (EXT-16)
. Added max-rate queue property (EXT-21)
. Enable deleting 
ow in all tables (EXT-10)
. Enable switch to check chaining when deleting groups (EXT-12)
. Enable controller to disable buering (EXT-45)
. Virtual ports renamed logical ports (EXT-78)
. New error messages (EXT-1, EXT-2, EXT-12, EXT-13, EXT-39, EXT-74 and EXT-82)
. Include release notes into the specication document
. Many other bug xes, rewording and clarications
122 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
B.11 OpenFlow version 1.3
Release date : April 13, 2012
Wire Protocol : 0x04
Please refers to the bug tracking ID for more details on each change
B.11.1 Refactor capabilities negotiation
Prior versions of the OpenFlow specication included limited expression of the capabilities of an Open-
Flow switch. OpenFlow 1.3 include a more 
exible framework to express capabilities (EXT-123).
The main change is the improved description of table capabilities. Those capabilities have been moved
out of the table statistics structure in its own request/reply message, and encoded using a 
exible TLV
format. This enables the additions of next-table capabilities, table-miss 
ow entry capabilities and
experimenter capabilities.
Other changes include renaming the 'stats' framework into the 'multipart' framework to re
ect the fact
that it is now used for both statistics and capabilities, and the move of port descriptions into its own
multipart message to enable support of a greater number of ports.
List of features for Refactor capabilities negotiation :
. Rename 'stats' framework into the 'multipart' framework.
. Enable 'multipart' requests (requests spanning multiple messages).
. Move port list description to its own multipart request/reply.
. Move table capabilities to its own multipart request/reply.
. Create 
exible property structure to express table capabilities.
. Enable to express experimenter capabilities.
. Add capabilities for table-miss 
ow entries.
. Add next-table (i.e. goto) capabilities
B.11.2 More 
exible table miss support
Prior versions of the OpenFlow specication included table conguration 
ags to select one of three 3
behaviour for handling table-misses (packet not matching any 
ows in the table). OpenFlow 1.3 replace
those limited 
ags with the table-miss 
ow entry, a special 
ow entry describing the behaviour on table
miss (EXT-108).
The table-miss 
ow entry uses standard OpenFlow instructions and actions to process table-miss packets,
this enables to use the full 
exibility of OpenFlow in processing those packets. All previous behaviour
expressed by the table-miss cong 
ags can be expressed using the table-miss 
ow entry. Many new
way of handling table-miss, such as processing table-miss with normal, can now trivially be described
by the OpenFlow protocol.
. Remove table-miss cong 
ags (EXT-108).
. Dene table-miss 
ow entry as the all wildcard, lowest priority 
ow entry (EXT-108).
. Mandate support of the table-miss 
ow entry in every table to process table-miss packets (EXT-
108).
123 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
. Add capabilities to describe the table-miss 
ow entry (EXT-123).
. Change table-miss default to drop packets (EXT-119).
B.11.3 IPv6 Extension Header handling support
Add the ability of match the presence of common IPv6 extension headers, and some anomalous conditions
in IPv6 extension headers (EXT-38). A new OXM pseudo header eld OXM_OF_IPV6_EXTHDR
enables to match the following conditions :
. Hop-by-hop IPv6 extension header is present.
. Router IPv6 extension header is present.
. Fragmentation IPv6 extension header is present.
. Destination options IPv6 extension headers is present.
. Authentication IPv6 extension header is present.
. Encrypted Security Payload IPv6 extension header is present.
. No Next Header IPv6 extension header is present.
. IPv6 extension headers out of preferred order.
. Unexpected IPv6 extension header encountered.
B.11.4 Per 
ow meters
Add support for per-
ow meters (EXT-14). Per-
ow meters can be attached to 
ow entries and can
measure and control the rate of packets. One of the main applications of per-
ow meters is to rate limit
packets sent to the controller.
The per-
ow meter feature is based on a new 
exible meter framework, which includes the ability to
describe complex meters through the use of multiple metering bands, metering statistics and capabilities.
Currently, only simple rate-limiter meters are dened over this framework. Support for color-aware
meters, which support Di-Serv style operation and are tightly integrated in the pipeline, was postponed
to a later release.
. Flexible meter framework based on per-
ow meters and meter bands.
. Meter statistics, including per band statistics.
. Enable to attach meters 
exibly to 
ow entries.
. Simple rate-limiter support (drop packets).
B.11.5 Per connection event ltering
Previous version of the specication introduced the ability for a switch to connect to multiple controllers
for fault tolerance and load balancing. Per connection event ltering improves the multi-controller
support by enabling each controller to lter events from the switch it does not want (EXT-120).
A new set of OpenFlow messages enables a controller to congure an event lter on its own connection
to the switch. Asynchronous messages can be ltered by type and reason. This event lter comes in
addition to other existing mechanisms that enable or disable asynchronous messages, for example the
generation of 
ow-removed events can be congured per 
ow. Each controller can have a separate lter
for the slave role and the master/equal role.
124 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
. Add asynchronous message lter for each controller connection.
. Controller message to set/get the asynchronous message lter.
. Set default lter value to match OpenFlow 1.2 behaviour.
. Remove OFPC_INVALID_TTL_TO_CONTROLLER cong 
ag.
B.11.6 Auxiliary connections
In previous version of the specication, the channel between the switch and the controller is exclusively
made of a single TCP connection, which does not allow to exploit the parallelism available in most
switch implementations. OpenFlow 1.3 enables a switch to create auxiliary connections to supplement
the main connection between the switch and the controller (EXT-114). Auxiliary connections are mostly
useful to carry packet-in and packet-out messages.
. Enable switch to create auxiliary connections to the controller.
. Mandate that auxiliary connection can not exist when main connection is not alive.
. Add auxiliary-id to the protocol to disambiguate the type of connection.
. Enable auxiliary connection over UDP and DTLS.
B.11.7 MPLS BoS matching
A new OXM eld OXM_OF_MPLS_BOS has been added to match the Bottom of Stack bit (BoS) from the
MPLS header (EXT-85). The BoS bit indicates if other MPLS shim header are in the payload of the
present MPLS packet, and matching this bit can help to disambiguate case where the MPLS label is
reused across levels of MPLS encapsulation.
B.11.8 Provider Backbone Bridging tagging
Add support for tagging packet using Provider Backbone Bridging (PBB) encapsulation (EXT-105).
This support enables OpenFlow to support various network deployment based on PBB, such as regular
PBB and PBB-TE.
. Push and Pop operation to add PBB header as a tag.
. New OXM eld to match I-SID for the PBB header.
B.11.9 Rework tag order
In previous version of the specication, the nal order of tags in a packet was statically specied. For
example, a MPLS shim header was always inserted after all VLAN tags in the packet. OpenFlow
1.3 removes this restriction, the nal order of tags in a packet is dictated by the order of the tagging
operations, each tagging operation adds its tag in the outermost position (EXT-121).
. Remove dened order of tags in packet from the specication.
. Tags are now always added in the outermost possible position.
. Action-list can add tags in arbitrary order.
. Tag order is predened for tagging in the action-set.
125 . 2012; The Open Networking Foundation
OpenFlow Switch Specication Version 1.3.1
B.11.10 Tunnel-ID metadata
The logical port abstraction enables OpenFlow to support a wide variety of encapsulations. The tunnelid
metadata OXM_OF_TUNNEL_ID is a new OXM eld that expose to the OpenFlow pipeline metadata
associated with the logical port, most commonly the demultiplexing eld from the encapsulation header
(EXT-107).
For example, if the logical port perform GRE encapsulation, the tunnel-id eld would map to the GRE
key eld from the GRE header. After decapsulation, OpenFlow would be able to match the GRE key
in the tunnel-id match eld. Similarly, by setting the tunnel-id, OpenFlow would be able to set the
GRE key in an encapsulated packet.
B.11.11 Cookies in packet-in
A cookie eld was added to the packet-in message (EXT-7). This eld takes its value from the 
ow the
sends the packet to the controller. If the packet was not sent by a 
ow, this eld is set to 0x.
Having the cookie in the packet-in enables the controller to more eciently classify packet-in, rather
than having to match the packet against the full 
ow table.

. Improve version negotiation using optional version bitmaps.





