/*A.3.5.2 Individual Flow Statistics
Information about individual 
ow entries is requested with the OFPMP_FLOW multipart request type:*/
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

/*The match eld contains a description of the 
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
The body of the reply to a OFPMP_FLOW multipart request consists of an array of the following:*/
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

/*The elds consist of those provided in the flow_mod that created the 
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
request type:*/

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

/*The elds in this message have the same meanings as in the individual 
ow stats request type
(OFPMP_FLOW).
The body of the reply consists of the following:*/
/* Body of reply to OFPMP_AGGREGATE request. */
struct ofp_aggregate_stats_reply {
    uint64_t packet_count; /* Number of packets in flows. */
    uint64_t byte_count; /* Number of bytes in flows. */
    uint32_t flow_count; /* Number of flows. */
    uint8_t pad[4]; /* Align to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp_aggregate_stats_reply) == 24);

/*A.3.5.4 Table Statistics
Information about tables is requested with the OFPMP_TABLE multipart request type. The request does
not contain any data in the body.
The body of the reply consists of an array of the following:*/
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

/*The array has one structure for each table supported by the switch. The entries are returned in the
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
The following structure describes the body of the table features request and reply:*/
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

/*The array has one structure for each table supported by the switch. The entries are returned in the
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
The list of table feature property types that are currently dened are:*/
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

/*The properties with the _MISS sux describe the capabilities for the table-miss 
ow entry (see 5.4),
whereas other properties describe the capabilities for regular 
ow entry. If a specic property does not
have any capability (for example no Set-Field support), a property with an empty list must be included
in the property list. When a property of the table-miss 
ow entry is the same as the corresponding
property for regular 
ow entries (i.e. both properties have the same list of capabilities), this table-miss
property can be omited from the property list.
A property denition contains the property type, length, and any associated data:*/
/* Common header for all Table Feature Properties */
struct ofp_table_feature_prop_header {
    uint16_t type; /* One of OFPTFPT_*. */
    uint16_t length; /* Length in bytes of this property. */
};
OFP_ASSERT(sizeof(struct ofp_table_feature_prop_header) == 4);
/*The OFPTFPT_INSTRUCTIONS and OFPTFPT_INSTRUCTIONS_MISS properties use the following structure
and elds:*/
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

/*The instruction_ids is the list of instructions supported by this table (see 5.9). The elements of that
list are variable size to enable expressing experimenter instructions, non-experimenter instructions are
4 bytes.
The OFPTFPT_NEXT_TABLES and OFPTFPT_NEXT_TABLES_MISS properties use the following structure and
elds:*/
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

/*The next_table_ids is the array of tables that can be directly reached from the present table using
the OFPIT_GOTO_TABLE instruction (see 5.1).
The OFPTFPT_WRITE_ACTIONS, OFPTFPT_WRITE_ACTIONS_MISS, OFPTFPT_APPLY_ACTIONS and
OFPTFPT_APPLY_ACTIONS_MISS properties use the following structure and elds:*/
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

/*The action_ids is the list of actions for the feature (see 5.12). The elements of that list are
variable size to enable expressing experimenter actions, non-experimenter actions are 4 bytes. The
OFPTFPT_WRITE_ACTIONS and OFPTFPT_WRITE_ACTIONS_MISS properties describe actions supported
by the table using the OFPIT_WRITE_ACTIONS instruction, whereas the OFPTFPT_APPLY_ACTIONS
and OFPTFPT_APPLY_ACTIONS_MISS properties describe actions supported by the table using the
OFPIT_APPLY_ACTIONS instruction.
The OFPTFPT_MATCH, OFPTFPT_WILDCARDS, OFPTFPT_WRITE_SETFIELD,
OFPTFPT_WRITE_SETFIELD_MISS, OFPTFPT_APPLY_SETFIELD and OFPTFPT_APPLY_SETFIELD_MISS
properties use the following structure and elds:*/
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
/*The oxm_ids is the list of OXM types for the feature (see A.2.3.2). The elements of that list are 32-bit
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
and elds:*/
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

/*The experimenter eld is the Experimenter ID, which takes the same form as in struct
ofp_experimenter (see A.5.4).
A.3.5.6 Port Statistics
Information about ports statistics is requested with the OFPMP_PORT_STATS multipart request type:*/
/* Body for ofp_multipart_request of type OFPMP_PORT. */
struct ofp_port_stats_request {
    uint32_t port_no; /* OFPMP_PORT message must request statistics
    * either for a single port (specified in
    * port_no) or for all ports (if port_no ==
    * OFPP_ANY). */
    uint8_t pad[4];
};
OFP_ASSERT(sizeof(struct ofp_port_stats_request) == 8);

/*The port_no eld optionally lters the stats request to the given port. To request all port statistics,
port_no must be set to OFPP_ANY.
The body of the reply consists of an array of the following:*/
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

/*The duration_sec and duration_nsec elds indicate the elapsed time the port has been congured
into the OpenFlow pipeline. The total duration in nanoseconds can be computed as duration sec 
109 + duration nsec. Implementations are required to provide second precision; higher precision is
encouraged where available.
A.3.5.7 Port Description
The port description request OFPMP_PORT_DESCRIPTION enables the controller to get a description of all
the ports in the system that support OpenFlow. The request body is empty. The reply body consists
of an array of the following:*/
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

/*This structure is described in section A.2.1.
A.3.5.8 Queue Statistics
The OFPMP_QUEUE multipart request message provides queue statistics for one or more ports and one
or more queues. The request body contains a port_no eld identifying the OpenFlow port for which
statistics are requested, or OFPP_ANY to refer to all ports. The queue_id eld identies one of the priority
queues, or OFPQ_ALL to refer to all queues congured at the specied port. OFPQ_ALL is 0x.*/
struct ofp_queue_stats_request {
    uint32_t port_no; /* All ports if OFPP_ANY. */
    uint32_t queue_id; /* All queues if OFPQ_ALL. */
};
OFP_ASSERT(sizeof(struct ofp_queue_stats_request) == 8);

/*The body of the reply consists of an array of the following structure:*/
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

/*The duration_sec and duration_nsec elds indicate the elapsed time the queue has been installed in
the switch. The total duration in nanoseconds can be computed as duration sec109+duration nsec.
Implementations are required to provide second precision; higher precision is encouraged where available.
A.3.5.9 Group Statistics
The OFPMP_GROUP multipart request message provides statistics for one or more groups. The request
body consists of a group_id eld, which can be set to OFPG_ALL to refer to all groups on the switch.*/
/* Body of OFPMP_GROUP request. */
struct ofp_group_stats_request {
    uint32_t group_id; /* All groups if OFPG_ALL. */
    uint8_t pad[4]; /* Align to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp_group_stats_request) == 8);

/*The body of the reply consists of an array of the following structure:*/
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

/*The elds consist of those provided in the group_mod that created the group, plus the ref_count
counting the number of 
ows referencing directly the group, the packet_count, and the byte_count
counting all packets processed by the group.
The duration_sec and duration_nsec elds indicate the elapsed time the group has been installed in
the switch. The total duration in nanoseconds can be computed as duration sec109+duration nsec.
Implementations are required to provide second precision; higher precision is encouraged where available.
The bucket_stats eld consists of an array of ofp_bucket_counter structs:*/
/* Used in group stats replies. */
struct ofp_bucket_counter {
    uint64_t packet_count; /* Number of packets processed by bucket. */
    uint64_t byte_count; /* Number of bytes processed by bucket. */
};
OFP_ASSERT(sizeof(struct ofp_bucket_counter) == 16);

/*A.3.5.10 Group Description
The OFPMP_GROUP_DESC multipart request message provides a way to list the set of groups on a switch,
along with their corresponding bucket actions. The request body is empty, while the reply body is an
array of the following structure:*/
/* Body of reply to OFPMP_GROUP_DESC request. */
struct ofp_group_desc_stats {
    uint16_t length; /* Length of this entry. */
    uint8_t type; /* One of OFPGT_*. */
    uint8_t pad; /* Pad to 64 bits. */
    uint32_t group_id; /* Group identifier. */
    struct ofp_bucket buckets[0];
};
OFP_ASSERT(sizeof(struct ofp_group_desc_stats) == 8);

/*Fields for group description are the same as those used with the ofp_group_mod struct (see A.3.4.2).
A.3.5.11 Group Features
The OFPMP_GROUP_FEATURES multipart request message provides a way to list the capabilities of groups
on a switch. The request body is empty, while the reply body is the following structure:*/

/* Body of reply to OFPMP_GROUP_FEATURES request. Group features. */
struct ofp_group_features {
    uint32_t types; /* Bitmap of OFPGT_* values supported. */
    uint32_t capabilities; /* Bitmap of OFPGFC_* capability supported. */
    uint32_t max_groups[4]; /* Maximum number of groups for each type. */
    uint32_t actions[4]; /* Bitmaps of OFPAT_* that are supported. */
};
OFP_ASSERT(sizeof(struct ofp_group_features) == 40);

/*The max_groups eld is the maximum number of groups for each type of groups. The actions is the
supported actions for each type of groups. The capabilities uses the following arg*/

/* Group configuration flags */
enum ofp_group_capabilities {
    OFPGFC_SELECT_WEIGHT = 1 << 0, /* Support weight for select groups */
    OFPGFC_SELECT_LIVENESS = 1 << 1, /* Support liveness for select groups */
    OFPGFC_CHAINING = 1 << 2, /* Support chaining groups */
    OFPGFC_CHAINING_CHECKS = 1 << 3, /* Check chaining for loops and delete */
};

/*A.3.5.12 Meter Statistics
The OFPMT_METER stats request message provides statistics for one or more meters. The request body
consists of a meter_id eld, which can be set to OFPM_ALL to refer to all meters on the switch.*/
/* Body of OFPMP_METER and OFPMP_METER_CONFIG requests. */
struct ofp_meter_multipart_request {
    uint32_t meter_id; /* Meter instance, or OFPM_ALL. */
    uint8_t pad[4]; /* Align to 64 bits. */
};

OFP_ASSERT(sizeof(struct ofp_meter_multipart_request) == 8);

/*The body of the reply consists of an array of the following structure:*/
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

/*The packet_in_count, and the byte_in_count count all packets processed by the meter.
The duration_sec and duration_nsec elds indicate the elapsed time the meter has been installed in
the switch. The total duration in nanoseconds can be computed as duration sec109+duration nsec.
Implementations are required to provide second precision; higher precision is encouraged where available.
The band_stats eld consists of an array of ofp_meter_band_stats structs:*/
/* Statistics for each meter band */
struct ofp_meter_band_stats {
    uint64_t packet_band_count; /* Number of packets in band. */
    uint64_t byte_band_count; /* Number of bytes in band. */
};
OFP_ASSERT(sizeof(struct ofp_meter_band_stats) == 16);

/*The packet_band_count, and the byte_band_count count all packets processed by the band.
The order of the band statistics must be the same as in the OFPMT_METER_CONFIG stats reply.
A.3.5.13 Meter Conguration Statistics
The OFPMT_METER_CONFIG stats request message provides conguration for one or more meter. The
request body consists of a meter_id eld, which can be set to OFPM_ALL to refer to all meters on the
switch.*/
/* Body of OFPMP_METER and OFPMP_METER_CONFIG requests. */
struct ofp_meter_multipart_request {
    uint32_t meter_id; /* Meter instance, or OFPM_ALL. */
    uint8_t pad[4]; /* Align to 64 bits. */
};
OFP_ASSERT(sizeof(struct ofp_meter_multipart_request) == 8);

/*The body of the reply consists of an array of the following structure:*/
/* Body of reply to OFPMP_METER_CONFIG request. Meter configuration. */
struct ofp_meter_config {
    uint16_t length; /* Length of this entry. */
    uint16_t flags; /* All OFPMC_* that apply. */
    uint32_t meter_id; /* Meter instance. */
    struct ofp_meter_band_header bands[0]; /* The bands length is
    inferred from the length field. */
};
OFP_ASSERT(sizeof(struct ofp_meter_config) == 8);

/*The elds are the same elds used for conguring the meter (see A.3.4.4).
A.3.5.14 Meter Features Statistics
The OFPMT_METER_FEATURES stats request message provides the set of features of the metering subsystem.
The request body is empty, and the body of the reply consists of the following structure:*/
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

