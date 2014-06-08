
/*A.3.4.4 Meter Modication Message
Modications to a meter from the controller are done with the OFPT_METER_MOD message:*/
/* Meter configuration. OFPT_METER_MOD. */
struct ofp_meter_mod {
    struct ofp_header header;
    uint16_t command; /* One of OFPMC_*. */
    uint16_t flags; /* One of OFPMF_*. */
    uint32_t meter_id; /* Meter instance. */
    struct ofp_meter_band_header bands[0]; /* The bands length is
    inferred from the length field
    in the header. */
};
OFP_ASSERT(sizeof(struct ofp_meter_mod) == 16);

/*The meter_id eld uniquely identies a meter within a switch. Meter are dened starting with
meter_id=1 up to the maximum number of meters that the switch can support. The OpenFlow
protocol also denes some additional virtual meters that can not be associated with 
ows:*/
/* Meter numbering. Flow meters can use any number up to OFPM_MAX. */
enum ofp_meter {
    /* Last usable meter. */
    OFPM_MAX = 0xffff0000,
    /* Virtual meters. */
    OFPM_SLOWPATH = 0xfffffffd, /* Meter for slow datapath. */
    OFPM_CONTROLLER = 0xfffffffe, /* Meter for controller connection. */
    OFPM_ALL = 0xffffffff, /* Represents all meters for stat requests
    commands. */
};

/*Virtual meters are provided to support existing implementations of OpenFlow. New implementations
are encouraged to use regular per-
ow meters or priority queues instead.
. OFPM_CONTROLLER: Virtual meter controlling all packets sent to the controller via Packet-in messages,
either using the CONTROLLER reserved port or in other processing (see 6.1.2). Can be
used to limit the amount of trac sent to the controller.
. OFPM_SLOWPATH: Virtual meter controlling all packets processed by the slow datapath of the switch.
Many switch implementations have a fast and slow datapath, for example a hardware switch may
have a slow software datapath, or a software switch may have a slow userspace datapath.
The command eld must be one of the following:*/
/* Meter commands */
enum ofp_meter_mod_command {
    OFPMC_ADD, /* New meter. */
    OFPMC_MODIFY, /* Modify specified meter. */
    OFPMC_DELETE, /* Delete specified meter. */
};

/*The flags eld must be one of the following:*/
/* Meter configuration flags */
enum ofp_meter_flags {
    OFPMF_KBPS = 1 << 0, /* Rate value in kb/s (kilo-bit per second). */
    OFPMF_PKTPS = 1 << 1, /* Rate value in packet/sec. */
    OFPMF_BURST = 1 << 2, /* Do burst size. */
    OFPMF_STATS = 1 << 3, /* Collect statistics. */
};

/*The bands eld is a list of rate bands. It can contain any number of bands, and each band type can be
repeated when it make sense. Only a single band is used at a time, if the current rate of packets exceed
the rate of multiple bands, the band with the highest congured rate is used.
All the rate bands are dened using the same common header:*/
/* Common header for all meter bands */
struct ofp_meter_band_header {
    uint16_t type; /* One of OFPMBT_*. */
    uint16_t len; /* Length in bytes of this band. */
    uint32_t rate; /* Rate for this band. */
    uint32_t burst_size; /* Size of bursts. */
};
OFP_ASSERT(sizeof(struct ofp_meter_band_header) == 12);

/*The rate eld indicates the rate value above which the corresponding band may apply to packets (see
5.7.1). The rate value is in kilobit per seconds, unless the flags eld includes OFPMF_PKTPS, in which
case the rate is in packets per seconds.
The burst_size eld is used only if the flags eld includes OFPMC_BURST. It indicate the length of the
packet or byte burst to consider for applying the meter. The burst value is in kilobits, unless the flags
eld includes OFPMF_PKTPS, in which case the burst value is in packets.
The type eld must be one of the following:*/
/* Meter band types */
enum ofp_meter_band_type {
    OFPMBT_DROP = 1, /* Drop packet. */
    OFPMBT_DSCP_REMARK = 2, /* Remark DSCP in the IP header. */
    OFPMBT_EXPERIMENTER = 0xFFFF /* Experimenter meter band. */
};

/*An OpenFlow switch may not support all band types, and may not allow to use all its supported bands
on all meters, i.e. some meters may be specialised.
The band OFPMBT_DROP denes a simple rate limiter that drop packets that exceed the band rate value,
and uses the following structure:*/
/* OFPMBT_DROP band - drop packets */
struct ofp_meter_band_drop {
    uint16_t type; /* OFPMBT_DROP. */
    uint16_t len; /* Length in bytes of this band. */
    uint32_t rate; /* Rate for dropping packets. */
    uint32_t burst_size; /* Size of bursts. */
    uint8_t pad[4];
};
OFP_ASSERT(sizeof(struct ofp_meter_band_drop) == 16);

/*The band OFPMBT_DSCP_REMARK denes a simple DiServ policer that remark the drop precedence of
the DSCP eld in the IP header of the packets that exceed the band rate value, and uses the following
structure:*/
/* OFPMBT_DSCP_REMARK band - Remark DSCP in the IP header */
struct ofp_meter_band_dscp_remark {
    uint16_t type; /* OFPMBT_DSCP_REMARK. */
    uint16_t len; /* Length in bytes of this band. */
    uint32_t rate; /* Rate for remarking packets. */
    uint32_t burst_size; /* Size of bursts. */
    uint8_t prec_level; /* Number of drop precedence level to add. */
    uint8_t pad[3];
};
OFP_ASSERT(sizeof(struct ofp_meter_band_dscp_remark) == 16);

/*The prec_level eld indicates by which amount the drop precedence of the packet should be increased
if the band is exceeded.
The band OFPMBT_EXPERIMENTER is experimenter dened and uses the following structure:*/
/* OFPMBT_EXPERIMENTER band - Write actions in action set */
struct ofp_meter_band_experimenter {
    uint16_t type; /* One of OFPMBT_*. */
    uint16_t len; /* Length in bytes of this band. */
    uint32_t rate; /* Rate for this band. */
    uint32_t burst_size; /* Size of bursts. */
    uint32_t experimenter; /* Experimenter ID which takes the same
    form as in struct
    ofp_experimenter_header. */
};
OFP_ASSERT(sizeof(struct ofp_meter_band_experimenter) == 16);
