#ifndef __ICE_H
#define __ICE_H

/* returns 0 if no ice 24 header was found */
int ice_24_header(unsigned char *src);

/* returns packed size of ice packed data */
long ice_24_packedsize(unsigned char *src);

/* returns origiginal size of ice packed data */
long ice_24_origsize(unsigned char *src);

/* Ice! V 2.4 depacker */
long ice_24_depack(unsigned char *src, unsigned char *dst);

#endif
