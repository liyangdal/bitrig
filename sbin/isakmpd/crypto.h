/*	$OpenBSD: crypto.h,v 1.7 2002/09/06 21:36:52 deraadt Exp $	*/
/*	$EOM: crypto.h,v 1.12 2000/10/15 21:56:41 niklas Exp $	*/

/*
 * Copyright (c) 1998 Niels Provos.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Ericsson Radio Systems.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This code was written under funding by Ericsson Radio Systems.
 */

#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#if defined (__APPLE__)

#include <openssl/des.h>
#ifdef USE_BLOWFISH
#include <openssl/blowfish.h>
#endif
#ifdef USE_CAST
#include <openssl/cast.h>
#endif

#else

#include <des.h>
#ifdef USE_BLOWFISH
#include <blf.h>
#endif
#ifdef USE_CAST
#include <cast.h>
#endif

#endif /* __APPLE__ */

#define USE_32BIT
#if defined (USE_64BIT)

#define XOR64(x,y) *(u_int64_t *)(x) ^= *(u_int64_t *)(y);
#define SET64(x,y) *(u_int64_t *)(x) = *(u_int64_t *)(y);

#elif defined (USE_32BIT)

#define XOR64(x,y) *(u_int32_t *)(x) ^= *(u_int32_t *)(y); \
   *(u_int32_t *)((u_int8_t *)(x) + 4) ^= *(u_int32_t *)((u_int8_t *)(y) + 4);
#define SET64(x,y) *(u_int32_t *)(x) = *(u_int32_t *)(y); \
   *(u_int32_t *)((u_int8_t *)(x) + 4) = *(u_int32_t *)((u_int8_t *)(y) + 4);

#else

#define XOR8(x,y,i) (x)[i] ^= (y)[i];
#define XOR64(x,y) XOR8(x,y,0); XOR8(x,y,1); XOR8(x,y,2); XOR8(x,y,3); \
   XOR8(x,y,4); XOR8(x,y,5); XOR8(x,y,6); XOR8(x,y,7);
#define SET8(x,y,i) (x)[i] = (y)[i];
#define SET64(x,y) SET8(x,y,0); SET8(x,y,1); SET8(x,y,2); SET8(x,y,3); \
   SET8(x,y,4); SET8(x,y,5); SET8(x,y,6); SET8(x,y,7);

#endif /* USE_64BIT */

#define SET_32BIT_BIG(x,y) (x)[3]= (y); (x)[2]= (y) >> 8; \
    (x)[1] = (y) >> 16; (x)[0]= (y) >> 24;
#define GET_32BIT_BIG(x) (u_int32_t)(x)[3] | ((u_int32_t)(x)[2] << 8) | \
    ((u_int32_t)(x)[1] << 16)| ((u_int32_t)(x)[0] << 24);

/*
 * This is standard for all block ciphers we use at the moment.
 * Theoretically this could increase in future, e.g. for TwoFish.
 * Keep MAXBLK uptodate
 */
#define BLOCKSIZE	8

#define MAXBLK		BLOCKSIZE

struct keystate {
  struct crypto_xf *xf;			/* Back pointer */
  u_int16_t	ebytes;			/* Number of encrypted bytes */
  u_int16_t	dbytes;			/* Number of decrypted bytes */
  time_t	life;			/* Creation time */
  u_int8_t	iv[MAXBLK];		/* Next IV to use */
  u_int8_t	iv2[MAXBLK];
  u_int8_t	*riv, *liv;
  union {
    des_key_schedule desks[3];
#ifdef USE_BLOWFISH
    blf_ctx blfks;
#endif
#ifdef USE_CAST
    cast_key castks;
#endif
  } keydata;
};

#define ks_des	keydata.desks
#define ks_blf	keydata.blfks
#define ks_cast	keydata.castks

/*
 * Information about the cryptotransform.
 *
 * XXX - In regards to the IV (Initialization Vector) the drafts are
 * completly fucked up and specify a MUST as how it is derived, so
 * we also have to provide for that. I just don't know where.
 * Furthermore is this enum needed at all?  It seems to be Oakley IDs
 * only anyhow, and we already have defines for that in ipsec_doi.h.
 */
enum transform {
  DES_CBC=1,			/* This is a MUST */
  IDEA_CBC=2,			/* Licensed, DONT use */
  BLOWFISH_CBC=3,
  RC5_R16_B64_CBC=4,		/* Licensed, DONT use */
  TRIPLEDES_CBC=5,			/* This is a SHOULD */
  CAST_CBC=6
};

enum cryptoerr {
  EOKAY,			/* No error */
  ENOCRYPTO,			/* A none crypto related error, see errno */
  EWEAKKEY,			/* A weak key was found in key setup */
  EKEYLEN			/* The key length was invalid for the cipher */
};

struct crypto_xf {
  enum transform id;		/* Oakley ID */
  char *name;			/* Transform Name */
  u_int16_t keymin, keymax;	/* Possible Keying Bytes */
  u_int16_t blocksize;		/* Need to keep IV in the state */
  struct keystate *state;	/* Key information, can also be passed sep. */
  enum cryptoerr (*init) (struct keystate *, u_int8_t *, u_int16_t);
  void (*encrypt) (struct keystate *, u_int8_t *, u_int16_t);
  void (*decrypt) (struct keystate *, u_int8_t *, u_int16_t);
};

extern struct keystate *crypto_clone_keystate (struct keystate *);
extern void crypto_decrypt (struct keystate *, u_int8_t *, u_int16_t);
extern void crypto_encrypt (struct keystate *, u_int8_t *, u_int16_t);
extern struct crypto_xf *crypto_get (enum transform);
extern struct keystate *crypto_init (struct crypto_xf *, u_int8_t *,
				     u_int16_t, enum cryptoerr *);
extern void crypto_init_iv (struct keystate *, u_int8_t *, size_t);
extern void crypto_update_iv (struct keystate *);

#endif /* _CRYPTO_H_ */
