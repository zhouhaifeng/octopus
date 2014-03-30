/* BGP-4 Link State Machine   
   From draft-ietf-idr-ls-distribution-04.pdf [Link-State Info Distribution using BGP]
   Copyright (C) 2014 zhouhaifeng

This file is part of GNU Zebra.

GNU Zebra is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

GNU Zebra is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Zebra; see the file COPYING.  If not, write to the Free
Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

struct bgp_nlri_ls
{
    u_int16_t type;
#define BGP_NLRI_LS_NODE                  1
#define BGP_NLRI_LS_LINK                   2
#define BGP_NLRI_LS_IPV4_PREFIX      3
#define BGP_NLRI_LS_IPV6_PREFIX      4

    u_int16_t length; 
    u_int8_t  *ls;
};

/*Node NLRI*/
struct bgp_nlri_ls_node
{
    u_int8_t     protocol_id;
#define BGP_NLRI_LS_ISIS_LEVEL1               1   /*The NLRI information has been sourced by IS-IS Level 1*/
#define BGP_NLRI_LS_ISIS_LEVEL2               2   /*The NLRI information has been sourced by IS-IS Level 2*/
#define BGP_NLRI_LS_OSPF                           3   /*The NLRI information has been sourced by OSPF*/
#define BGP_NLRI_LS_DIRECT                        4   /*The NLRI information has been sourced from local interface state*/
#define BGP_NLRI_LS_STATIC                        5   /*The NLRI information has been sourced by static configuration*/

    u_int32_t   id[2];               /*Identifier*/
#define BGP_NLRI_LS_L3_TOPO                      0  /*L3 packet topology |*/
#define BGP_NLRI_LS_L1_TOPO                      1  /*L1 optical topology |*/

    u_int32_t   as;
    u_int32_t   id;
    u_int32_t   area_id;
    u_int32_t   router_id;
};

/*Node Descriptor Sub-TLVs*/
struct bgp_nlri_ls_node_desc
{
    u_int16_t    type;
#define BGP_NLRI_LS_AS                                    512  /*Autonomous System*/
#define BGP_NLRI_LS_ID                                    513  /*BGP-LS ID*/
#define BGP_NLRI_LS_AREA_ID                          514  /*Area ID*/
#define BGP_NLRI_LS_ROUTER_ID                      515  /*IGP Router-ID*/
    u_int16_t    length;
    u_int8_t    *desc;
};

/*Link NLRI*/
struct bgp_nlri_ls_link
{
    u_int8_t     protocol_id;
    u_int32_t   id[2];
    u_int8_t   *desc;
};

/*Prefix NLRI*/
struct bgp_nlri_ls_prefix
{
    u_int8_t     protocol_id;
    u_int32_t   id[2];
    u_int8_t   *desc;
};

static struct stream *bgp_ls_update_packet (struct peer *peer, afi_t afi, safi_t safi);
bgp_size_t bgp_packet_attribute (struct bgp *bgp, struct peer *peer,
                                        		      struct stream *s, struct attr *attr, 
                                        		      afi_t afi, safi_t safi, struct peer *from,
                                        		      struct prefix_rd *prd, u_char *tag);

