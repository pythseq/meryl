
/******************************************************************************
 *
 *  This file is part of 'sequence' and/or 'meryl', software programs for
 *  working with DNA sequence files and k-mers contained in them.
 *
 *  Modifications by:
 *
 *    Brian P. Walenz beginning on 2018-FEB-26
 *      are a 'United States Government Work', and
 *      are released in the public domain
 *
 *  File 'README.license' in the root directory of this distribution contains
 *  full conditions and disclaimers.
 */

#include "libbits.H"
#include "mt19937ar.H"

char          b1[65];
char          b2[65];
char          b3[65];


void
testLogBaseTwo(void) {
  uint64  val = 0;

  for (uint32 ii=0; ii<64; ii++) {
    assert(ii == logBaseTwo64(val));

    val <<= 1;
    //val++;

    if (val == 0)
      val = 1;
  }
}


void
testClear(void) {
  uint64  bb = 0xffffffffffffffffllu;

  for (uint32 ii=0; ii<65; ii++)
    fprintf(stderr, "%2u clearLeft %s clearRight %s\n",
            ii,
            displayWord(clearLeftBits (bb, ii), b1),
            displayWord(clearRightBits(bb, ii), b2));

  for (uint32 ii=0; ii<65; ii++)
    fprintf(stderr, "%2u  saveLeft %s  saveRight %s\n",
            ii,
            displayWord(saveLeftBits (bb, ii), b1),
            displayWord(saveRightBits(bb, ii), b2));

  for (uint32 ii=0; ii<65; ii++)
    fprintf(stderr, "%2u  saveMid  %s clearMid   %s\n",
            ii,
            displayWord(saveMiddleBits (bb, ii, 10), b1),
            displayWord(clearMiddleBits(bb, ii, 10), b2));

  for (uint32 ii=0; ii<65; ii++)
    fprintf(stderr, "%2u  saveMid  %s clearMid   %s\n",
            ii,
            displayWord(saveMiddleBits (bb, 10, ii), b1),
            displayWord(clearMiddleBits(bb, 10, ii), b2));
}


void
testUnary(uint32 testSize) {
  uint32      maxN   = 10000000;
  uint32     *random = new uint32 [maxN];
  mtRandom    mt;

  fprintf(stderr, "Creating.\n");

  uint64  total = 0;

  for (uint32 ii=0; ii<maxN; ii++) {
    random[ii]  = mt.mtRandom32() % testSize;
    total      += random[ii] + 1;
  }

  fprintf(stderr, "Created %lu Mbits, %lu MB needed.\n", total >> 20, (total + maxN * sizeof(uint32) * 8) >> 23);

  total = 0;

  fprintf(stderr, "Setting.\n");

  stuffedBits *bits = new stuffedBits;

  for (uint32 ii=0; ii<maxN; ii++) {
    bits->setUnary(random[ii]);
    total += random[ii] + 1;

    assert(bits->getPosition() == total);
  }

  fprintf(stderr, "Testing.\n");

  bits->setPosition(0);

  for (uint32 ii=0; ii<maxN; ii++)
    assert(random[ii] == bits->getUnary());

  fprintf(stderr, "Tested.\n");

  //while (1)
  //  ;

  delete    bits;
  delete [] random;
}





void
testBinary(uint32 testSize) {
  uint32      maxN   = 10000000;
  uint32     *width  = new uint32 [maxN];
  uint64     *random = new uint64 [maxN];
  mtRandom    mt;

  fprintf(stderr, "Creating.\n");

  uint64  total = 0;

  for (uint32 ii=0; ii<maxN; ii++) {
    width[ii]   = mt.mtRandom32() % testSize;
    random[ii]  = mt.mtRandom64() & (((uint64)1 << width[ii]) - 1);
    total      +=  width[ii];
  }

  fprintf(stderr, "Created %lu Mbits, %lu MB needed.\n", total >> 20, (total + maxN * sizeof(uint32) * 8) >> 23);

  total = 0;

  fprintf(stderr, "Setting.\n");

  stuffedBits *bits = new stuffedBits;

  for (uint32 ii=0; ii<maxN; ii++) {
    bits->setBinary(width[ii], random[ii]);
    total += width[ii];

    assert(bits->getPosition() == total);
  }

  fprintf(stderr, "Testing.\n");

  bits->setPosition(0);

  for (uint32 ii=0; ii<maxN; ii++) {
    uint64  b = bits->getBinary(width[ii]);
    assert(random[ii] == b);
  }

  fprintf(stderr, "Tested.\n");

  //while (1)
  //  ;

  delete    bits;
  delete [] random;
  delete [] width;
}





int
main(int argc, char **argv) {

  //testLogBaseTwo();
  //testClear();

  //#pragma omp parallel for
  //for (uint32 xx=1; xx<2000; xx++) {
  //  fprintf(stderr, "TESTING %u out of 2000.\n", xx);
  //  testUnary(xx);
  //}

  //#pragma omp parallel for
  for (uint32 xx=1; xx<65; xx++) {
    fprintf(stderr, "TESTING %u out of 2000.\n", xx);
    testBinary(xx);
  }

  exit(0);
}
