/*Copyright 2013 haifeng.zhou, email: haifeng.zhou@live.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

/* Header on all OpenFlow packets. */
struct ofp_header {
    uint8_t version; /* OFP_VERSION. */
    uint8_t type; /* One of the OFPT_ constants. */
    uint16_t length; /* Length including this ofp_header. */
    uint32_t xid; /* Transaction id associated with this packet.
    Replies use the same id as was in the request
    to facilitate pairing. */
};

enum ofp_type {
    /* Immutable messages. */
    OFPT_HELLO = 0, /* Symmetric message */
    OFPT_ERROR = 1, /* Symmetric message */
    OFPT_ECHO_REQUEST = 2, /* Symmetric message */
    OFPT_ECHO_REPLY = 3, /* Symmetric message */
    OFPT_EXPERIMENTER = 4, /* Symmetric message */
    /* Switch configuration messages. */
    OFPT_FEATURES_REQUEST = 5, /* Controller/switch message */
    OFPT_FEATURES_REPLY = 6, /* Controller/switch message */
    OFPT_GET_CONFIG_REQUEST = 7, /* Controller/switch message */
    OFPT_GET_CONFIG_REPLY = 8, /* Controller/switch message */
    OFPT_SET_CONFIG = 9, /* Controller/switch message */
    /* Asynchronous messages. */
    OFPT_PACKET_IN = 10, /* Async message */
    OFPT_FLOW_REMOVED = 11, /* Async message */
    OFPT_PORT_STATUS = 12, /* Async message */
    /* Controller command messages. */
    OFPT_PACKET_OUT = 13, /* Controller/switch message */
    OFPT_FLOW_MOD = 14, /* Controller/switch message */
    OFPT_GROUP_MOD = 15, /* Controller/switch message */
    OFPT_PORT_MOD = 16, /* Controller/switch message */
    OFPT_TABLE_MOD = 17, /* Controller/switch message */
    /* Multipart messages. */
    OFPT_MULTIPART_REQUEST = 18, /* Controller/switch message */
    OFPT_MULTIPART_REPLY = 19, /* Controller/switch message */
    /* Barrier messages. */
    OFPT_BARRIER_REQUEST = 20, /* Controller/switch message */
    OFPT_BARRIER_REPLY = 21, /* Controller/switch message */
    /* Queue Configuration messages. */
    OFPT_QUEUE_GET_CONFIG_REQUEST = 22, /* Controller/switch message */
    OFPT_QUEUE_GET_CONFIG_REPLY = 23, /* Controller/switch message */
    /* Controller role change request messages. */
    OFPT_ROLE_REQUEST = 24, /* Controller/switch message */
    OFPT_ROLE_REPLY = 25, /* Controller/switch message */
    /* Asynchronous message configuration. */
    OFPT_GET_ASYNC_REQUEST = 26, /* Controller/switch message */
    OFPT_GET_ASYNC_REPLY = 27, /* Controller/switch message */
    OFPT_SET_ASYNC = 28, /* Controller/switch message */
    /* Meters and rate limiters configuration messages. */
    OFPT_METER_MOD = 29, /* Controller/switch message */
};

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

/*The port_no eld uniquely identies a port within a switch. The hw_addr eld typically is the MAC
address for the port; OFP_MAX_ETH_ALEN is 6. The name eld is a null-terminated string containing a
human-readable name for the interface. The value of OFP_MAX_PORT_NAME_LEN is 16.
The config eld describes port administrative settings, and has the following structure:*/

/* Flags to indicate behavior of the physical port. These flags are
* used in ofp_port to describe the current configuration. They are
* used in the ofp_port_mod message to configure the port's behavior.
*/
enum ofp_port_config {
    OFPPC_PORT_DOWN = 1 << 0, /* Port is administratively down. */
    OFPPC_NO_RECV = 1 << 2, /* Drop all packets received by port. */
    OFPPC_NO_FWD = 1 << 5, /* Drop packets forwarded to port. */
    OFPPC_NO_PACKET_IN = 1 << 6 /* Do not send packet-in msgs for port. */
};

/*The OFPPC_PORT_DOWN bit indicates that the port has been administratively brought down and should
not be used by OpenFlow. The OFPPC_NO_RECV bit indicates that packets received on that port should
be ignored. The OFPPC_NO_FWD bit indicates that OpenFlow should not send packets to that port. The
OFPPFL_NO_PACKET_IN bit indicates that packets on that port that generate a table miss should never
trigger a packet-in message to the controller.
In general, the port cong bits are set by the controller and not changed by the switch. Those bits
may be useful for the controller to implement protocols such as STP or BFD. If the port cong bits are
changed by the switch through another administrative interface, the switch sends an OFPT_PORT_STATUS
message to notify the controller of the change.
The state eld describes the port internal state, and has the following structure:*/

/* Current state of the physical port. These are not configurable from
* the controller.
*/
enum ofp_port_state {
    OFPPS_LINK_DOWN = 1 << 0, /* No physical link present. */
    OFPPS_BLOCKED = 1 << 1, /* Port is blocked */
    OFPPS_LIVE = 1 << 2, /* Live for Fast Failover Group. */
};
/*The port state bits represent the state of the physical link or switch protocols outside of OpenFlow. The
OFPPS_LINK_DOWN bit indicates the the physical link is not present. The OFPPS_BLOCKED bit indicates
that a switch protocol outside of OpenFlow, such as 802.1D Spanning Tree, is preventing the use of
that port with OFPP_FLOOD.
All port state bits are read-only and cannot be changed by the controller. When the port 
ags are
changed, the switch sends an OFPT_PORT_STATUS message to notify the controller of the change.
The port numbers use the following conventions:*/

/* Port numbering. Ports are numbered starting from 1. */
enum ofp_port_no {
    /* Maximum number of physical and logical switch ports. */
    OFPP_MAX = 0xffffff00,
    /* Reserved OpenFlow Port (fake output "ports"). */
    OFPP_IN_PORT = 0xfffffff8, /* Send the packet out the input port. This
    reserved port must be explicitly used
    in order to send back out of the input
    port. */
    OFPP_TABLE = 0xfffffff9, /* Submit the packet to the first flow table
    NB: This destination port can only be
    used in packet-out messages. */
    OFPP_NORMAL = 0xfffffffa, /* Process with normal L2/L3 switching. */
    OFPP_FLOOD = 0xfffffffb, /* All physical ports in VLAN, except input
    port and those blocked or link down. */
    OFPP_ALL = 0xfffffffc, /* All physical ports except input port. */
    OFPP_CONTROLLER = 0xfffffffd, /* Send to controller. */
    OFPP_LOCAL = 0xfffffffe, /* Local openflow "port". */
    OFPP_ANY = 0xffffffff /* Wildcard port used only for flow mod
    (delete) and flow stats requests. Selects
    all flows regardless of output port
    (including flows with no output port). */
};

/*The curr, advertised, supported, and peer elds indicate link modes (speed and duplexity), link
type (copper/ber) and link features (autonegotiation and pause). Port features are represented by the
following structure:*/

/* Features of ports available in a datapath. */
enum ofp_port_features {
    OFPPF_10MB_HD = 1 << 0, /* 10 Mb half-duplex rate support. */
    OFPPF_10MB_FD = 1 << 1, /* 10 Mb full-duplex rate support. */
    OFPPF_100MB_HD = 1 << 2, /* 100 Mb half-duplex rate support. */
    OFPPF_100MB_FD = 1 << 3, /* 100 Mb full-duplex rate support. */
    OFPPF_1GB_HD = 1 << 4, /* 1 Gb half-duplex rate support. */
    OFPPF_1GB_FD = 1 << 5, /* 1 Gb full-duplex rate support. */
    OFPPF_10GB_FD = 1 << 6, /* 10 Gb full-duplex rate support. */
    OFPPF_40GB_FD = 1 << 7, /* 40 Gb full-duplex rate support. */
    OFPPF_100GB_FD = 1 << 8, /* 100 Gb full-duplex rate support. */
    OFPPF_1TB_FD = 1 << 9, /* 1 Tb full-duplex rate support. */
    OFPPF_OTHER = 1 << 10, /* Other rate, not in the list. */
    OFPPF_COPPER = 1 << 11, /* Copper medium. */
    OFPPF_FIBER = 1 << 12, /* Fiber medium. */
    OFPPF_AUTONEG = 1 << 13, /* Auto-negotiation. */
    OFPPF_PAUSE = 1 << 14, /* Pause. */
    OFPPF_PAUSE_ASYM = 1 << 15 /* Asymmetric pause. */
};

/*Multiple of these 
ags may be set simultaneously. If none of the port speed 
ags are set, the max_speed
or curr_speed are used.
The curr_speed and max_speed elds indicate the current and maximum bit rate (raw transmission
speed) of the link in kbps. The number should be rounded to match common usage. For example,
an optical 10 Gb Ethernet port should have this eld set to 10000000 (instead of 10312500), and an
OC-192 port should have this eld set to 10000000 (instead of 9953280).
The max_speed elds indicate the maximum congured capacity of the link, whereas the curr_speed
indicates the current capacity. If the port is a LAG with 3 links of 1Gb/s capacity, with one of the ports
of the LAG being down, one port auto-negotiated at 1Gb/s and 1 port auto-negotiated at 100Mb/s,
the max_speed is 3 Gb/s and the curr_speed is 1.1 Gb/s.
A.2.2 Queue Structures
An OpenFlow switch provides limited Quality-of-Service support (QoS) through a simple queuing mechanism.
One (or more) queues can attach to a port and be used to map 
ow entries on it. Flow entries
mapped to a specic queue will be treated according to that queue's conguration (e.g. min rate).
A queue is described by the ofp_packet_queue structure:*/

/* Full description for a queue. */
struct ofp_packet_queue {
    uint32_t queue_id; /* id for the specific queue. */
    uint32_t port; /* Port this queue is attached to. */
    uint16_t len; /* Length in bytes of this queue desc. */
    uint8_t pad[6]; /* 64-bit alignment. */
    struct ofp_queue_prop_header properties[0]; /* List of properties. */
};

OFP_ASSERT(sizeof(struct ofp_packet_queue) == 16);

/*Each queue is further described by a set of properties, each of a specic type and conguration.*/
enum ofp_queue_properties {
    OFPQT_MIN_RATE = 1, /* Minimum datarate guaranteed. */
    OFPQT_MAX_RATE = 2, /* Maximum datarate. */
    OFPQT_EXPERIMENTER = 0xffff /* Experimenter defined property. */
};

/*Each queue property description starts with a common header:*/
/* Common description for a queue. */
struct ofp_queue_prop_header {
    uint16_t property; /* One of OFPQT_. */
    uint16_t len; /* Length of property, including this header. */
    uint8_t pad[4]; /* 64-bit alignemnt. */
};

OFP_ASSERT(sizeof(struct ofp_queue_prop_header) == 8);
/*A minimum-rate queue property uses the following structure and elds:*/

/* Min-Rate queue property description. */
struct ofp_queue_prop_min_rate {
    struct ofp_queue_prop_header prop_header; /* prop: OFPQT_MIN, len: 16. */
    uint16_t rate; /* In 1/10 of a percent; >1000 -> disabled. */
    uint8_t pad[6]; /* 64-bit alignment */
};

OFP_ASSERT(sizeof(struct ofp_queue_prop_min_rate) == 16);

/*If rate is not congured, it is set to OFPQ_MIN_RATE_UNCFG, which is 0x.
A maximum-rate queue property uses the following structure and elds:*/

/* Max-Rate queue property description. */
struct ofp_queue_prop_max_rate {
    struct ofp_queue_prop_header prop_header; /* prop: OFPQT_MAX, len: 16. */
    uint16_t rate; /* In 1/10 of a percent; >1000 -> disabled. */
    uint8_t pad[6]; /* 64-bit alignment */
};

OFP_ASSERT(sizeof(struct ofp_queue_prop_max_rate) == 16);
/*If rate is not congured, it is set to OFPQ_MAX_RATE_UNCFG, which is 0x.
A experimenter queue property uses the following structure and elds:*/

/* Experimenter queue property description. */
struct ofp_queue_prop_experimenter {
    struct ofp_queue_prop_header prop_header; /* prop: OFPQT_EXPERIMENTER, len: 16. */
    uint32_t experimenter; /* Experimenter ID which takes the same
    form as in struct
    ofp_experimenter_header. */
    uint8_t pad[4]; /* 64-bit alignment */
    uint8_t data[0]; /* Experimenter defined data. */
};
OFP_ASSERT(sizeof(struct ofp_queue_prop_experimenter) == 16);

/*The rest of the experimenter queue property body is uninterpreted by standard OpenFlow processing
and is arbitrarily dened by the corresponding experimenter.*/

