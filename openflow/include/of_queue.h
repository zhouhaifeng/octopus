
/*The rest of the request and reply bodies are experimenter-dened.
The experimenter eld is the Experimenter ID, which takes the same form as in struct
ofp_experimenter (see A.5.4).
A.3.6 Queue Conguration Messages
Queue conguration takes place outside the OpenFlow protocol, either through a command line tool or
through an external dedicated conguration protocol.
The controller can query the switch for congured queues on a port using the following structure:*/
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

/*The switch replies back with an ofp_queue_get_config_reply command, containing a list of congured
queues.*/
/* Queue configuration for a given port. */
struct ofp_queue_get_config_reply {
    struct ofp_header header;
    uint32_t port;
    uint8_t pad[4];
    struct ofp_packet_queue queues[0]; /* List of configured queues. */
};
OFP_ASSERT(sizeof(struct ofp_queue_get_config_reply) == 16);
