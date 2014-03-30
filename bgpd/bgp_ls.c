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

static struct stream *bgp_ls_update_packet (struct peer *peer, afi_t afi, safi_t safi)
{
    struct stream *s;
    struct bgp_adj_out *adj;
    struct bgp_advertise *adv;
    struct stream *packet;
    struct bgp_node *rn = NULL;
    struct bgp_info *binfo = NULL;
    bgp_size_t total_attr_len = 0;
    unsigned long pos;

    s = peer->work;
    stream_reset (s);

    adv = FIFO_HEAD (&peer->sync[afi][safi]->ls_update);

    while (adv)
    {
        if (adv->node)
            binfo = adv->binfo;

        /* When remaining space can't include NLRI and it's length.  */
        if (STREAM_REMAIN (s) <= BGP_NLRI_LENGTH + PSIZE (rn->p.prefixlen))
	    break;

      /* If packet is empty, set attribute. */
      if (stream_empty (s))
	{
	  struct prefix_rd *prd = NULL;
	  u_char *tag = NULL;
	  struct peer *from = NULL;
	  
	  if (rn->prn)
	    prd = (struct prefix_rd *) &rn->prn->p;
          if (binfo)
            {
              from = binfo->peer;
              if (binfo->extra)
                tag = binfo->extra->tag;
            }
          
	  bgp_packet_set_marker (s, BGP_MSG_UPDATE);
	  stream_putw (s, 0);		
	  pos = stream_get_endp (s);
	  stream_putw (s, 0);
	  total_attr_len = bgp_packet_attribute (NULL, peer, s, 
	                                         adv->baa->attr,
	                                         &rn->p, afi, safi, 
	                                         from, prd, tag);
	  stream_putw_at (s, pos, total_attr_len);
	}

      if (afi == AFI_IP && safi == SAFI_UNICAST)
	stream_put_prefix (s, &rn->p);
      
      if (BGP_DEBUG (update, UPDATE_OUT))
        {
          char buf[INET6_BUFSIZ];

          zlog (peer->log, LOG_DEBUG, "%s send UPDATE %s/%d",
                peer->host,
                inet_ntop (rn->p.family, &(rn->p.u.prefix), buf, INET6_BUFSIZ),
                rn->p.prefixlen);
        }

      /* Synchnorize attribute.  */
      if (adj->attr)
	bgp_attr_unintern (&adj->attr);
      else
	peer->scount[afi][safi]++;

      adj->attr = bgp_attr_intern (adv->baa->attr);

      adv = bgp_advertise_clean (peer, adj, afi, safi);

      if (! (afi == AFI_IP && safi == SAFI_UNICAST))
	break;
    }
	 
  if (! stream_empty (s))
    {
      bgp_packet_set_size (s);
      packet = stream_dup (s);
      bgp_packet_add (peer, packet);
      BGP_WRITE_ON (peer->t_write, bgp_write, peer->fd);
      stream_reset (s);
      return packet;
    }
  return NULL;
}
