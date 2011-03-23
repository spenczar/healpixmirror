/*
 *  This file is part of Healpix_cxx.
 *
 *  Healpix_cxx is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Healpix_cxx is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Healpix_cxx; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  For more information about HEALPix, see http://healpix.jpl.nasa.gov
 */

/*
 *  Healpix_cxx is being developed at the Max-Planck-Institut fuer Astrophysik
 *  and financially supported by the Deutsches Zentrum fuer Luft- und Raumfahrt
 *  (DLR).
 */

/*
 *  Copyright (C) 2004-2011 Max-Planck-Society
 *  Author: Martin Reinecke
 */

/*

Candidates for testing the validity of the Healpix routines:

- done: ang2pix(pix2ang(i)) = i for all Healpix_Bases
- done: pix2ang(ang2pix(ptg)) dot ptg > 1-delta for all Healpix_Bases
- done: ring2nest(nest2ring(i)) = i for all hierarchical Healpix_Bases
- done: downgrade(upgrade(map)) = map for all maps
- done: map and downgraded map should have same average
- done: alm2map(map2alm(map)) approx map (same for pol)
- partly done: neighbor tests
- powspec -> alm -> powspec (should produce similar powspecs, also for pol)
- done: two swap_schemes() should produce original map
- done: query_disc tests (dot products etc.)
- a_lms: test Set(), Scale(), Add(), alm(l,m) = alm.mstart(m)[l], etc.

*/

#include <iostream>
#include "healpix_base.h"
#include "healpix_base2.h"
#include "healpix_map.h"
#include "arr.h"
#include "planck_rng.h"
#include "lsconstants.h"
#include "alm.h"
#include "alm_healpix_tools.h"
#include "alm_powspec_tools.h"
#include "geom_utils.h"
#include "walltimer.h"

using namespace std;

const int nsamples = 1000000;

planck_rng rng;

namespace {

void random_dir (pointing &ptg)
  {
  ptg.theta = acos(rng.rand_uni()*2-1);
  ptg.phi = rng.rand_uni()*twopi;
  }

void check_ringnestring()
  {
  cout << "testing ring2nest(nest2ring(m))==m" << endl;
  for (int order=0; order<=13; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Base base (order,RING);
    for (int m=0; m<nsamples; ++m)
      {
      int pix = int(rng.rand_uni()*base.Npix());
      if (base.ring2nest(base.nest2ring(pix))!=pix)
        cout << "  PROBLEM: order = " << order << ", pixel = " << pix << endl;
      }
    }
  }
void check_ringnestring2()
  {
  cout << "testing ring2nest(nest2ring(m))==m" << endl;
  for (int order=0; order<=29; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Base2 base (order,RING);
    for (int m=0; m<nsamples; ++m)
      {
      int64 pix = int64(rng.rand_uni()*base.Npix());
      if (base.ring2nest(base.nest2ring(pix))!=pix)
        cout << "  PROBLEM: order = " << order << ", pixel = " << pix << endl;
      }
    }
  }

void check_nestpeanonest()
  {
  cout << "testing peano2nest(nest2peano(m))==m" << endl;
  for (int order=0; order<=13; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Base base (order,NEST);
    for (int m=0; m<nsamples; ++m)
      {
      int pix = int(rng.rand_uni()*base.Npix());
      if (base.peano2nest(base.nest2peano(pix))!=pix)
        cout << "  PROBLEM: order = " << order << ", pixel = " << pix << endl;
      }
    }
  }
void check_nestpeanonest2()
  {
  cout << "testing peano2nest(nest2peano(m))==m" << endl;
  for (int order=0; order<=29; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Base2 base (order,NEST);
    for (int m=0; m<nsamples; ++m)
      {
      int64 pix = int64(rng.rand_uni()*base.Npix());
      if (base.peano2nest(base.nest2peano(pix))!=pix)
        cout << "  PROBLEM: order = " << order << ", pixel = " << pix << endl;
      }
    }
  }

void check_pixangpix()
  {
  cout << "testing ang2pix(pix2ang(m))==m" << endl;
  for (int order=0; order<=13; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Base base1 (order,RING);
    Healpix_Base base2 (order,NEST);
    for (int m=0; m<nsamples; ++m)
      {
      int pix = int(rng.rand_uni()*base1.Npix());
      if (base1.ang2pix(base1.pix2ang(pix))!=pix)
        cout << "  PROBLEM: order = " << order << ", pixel = " << pix << endl;
      if (base2.ang2pix(base2.pix2ang(pix))!=pix)
        cout << "  PROBLEM: order = " << order << ", pixel = " << pix << endl;
      }
    }
  for (int nside=3; nside<(1<<13); nside+=nside/2+1)
    {
    cout << "nside = " << nside << endl;
    Healpix_Base base (nside,RING,SET_NSIDE);
    for (int m=0; m<nsamples; ++m)
      {
      int pix = int(rng.rand_uni()*base.Npix());
      if (base.ang2pix(base.pix2ang(pix))!=pix)
        cout << "  PROBLEM: nside = " << nside << ", pixel = " << pix << endl;
      }
    }
  }
void check_pixangpix2()
  {
  cout << "testing ang2pix(pix2ang(m))==m" << endl;
  for (int order=0; order<=29; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Base2 base1 (order,RING);
    Healpix_Base2 base2 (order,NEST);
    for (int m=0; m<nsamples; ++m)
      {
      int64 pix = int64(rng.rand_uni()*base1.Npix());
      if (base1.ang2pix(base1.pix2ang(pix))!=pix)
        cout << "  PROBLEM: order = " << order << ", pixel = " << pix << endl;
      if (base2.ang2pix(base2.pix2ang(pix))!=pix)
        cout << "  PROBLEM: order = " << order << ", pixel = " << pix << endl;
      }
    }
  for (int nside=3; nside<(1<<29); nside+=nside/2+1)
    {
    cout << "nside = " << nside << endl;
    Healpix_Base2 base (nside,RING,SET_NSIDE);
    for (int m=0; m<nsamples; ++m)
      {
      int64 pix = int64(rng.rand_uni()*base.Npix());
      if (base.ang2pix(base.pix2ang(pix))!=pix)
        cout << "  PROBLEM: nside = " << nside << ", pixel = " << pix << endl;
      }
    }
  }

void check_pixvecpix()
  {
  cout << "testing vec2pix(pix2vec(m))==m" << endl;
  for (int order=0; order<=13; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Base base1 (order,RING);
    Healpix_Base base2 (order,NEST);
    for (int m=0; m<nsamples; ++m)
      {
      int pix = int(rng.rand_uni()*base1.Npix());
      if (base1.vec2pix(base1.pix2vec(pix))!=pix)
        cout << "  PROBLEM: order = " << order << ", pixel = " << pix << endl;
      if (base2.vec2pix(base2.pix2vec(pix))!=pix)
        cout << "  PROBLEM: order = " << order << ", pixel = " << pix << endl;
      }
    }
  for (int nside=3; nside<(1<<13); nside+=nside/2+1)
    {
    cout << "nside = " << nside << endl;
    Healpix_Base base (nside,RING,SET_NSIDE);
    for (int m=0; m<nsamples; ++m)
      {
      int pix = int(rng.rand_uni()*base.Npix());
      if (base.vec2pix(base.pix2vec(pix))!=pix)
        cout << "  PROBLEM: nside = " << nside << ", pixel = " << pix << endl;
      }
    }
  }
void check_pixvecpix2()
  {
  cout << "testing vec2pix(pix2vec(m))==m" << endl;
  for (int order=0; order<=29; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Base2 base1 (order,RING);
    Healpix_Base2 base2 (order,NEST);
    for (int m=0; m<nsamples; ++m)
      {
      int64 pix = int64(rng.rand_uni()*base1.Npix());
      if (base1.vec2pix(base1.pix2vec(pix))!=pix)
        cout << "  PROBLEM: order = " << order << ", pixel = " << pix << endl;
      if (base2.vec2pix(base2.pix2vec(pix))!=pix)
        cout << "  PROBLEM: order = " << order << ", pixel = " << pix << endl;
      }
    }
  for (int nside=3; nside<(1<<29); nside+=nside/2+1)
    {
    cout << "nside = " << nside << endl;
    Healpix_Base2 base (nside,RING,SET_NSIDE);
    for (int m=0; m<nsamples; ++m)
      {
      int64 pix = int64(rng.rand_uni()*base.Npix());
      if (base.vec2pix(base.pix2vec(pix))!=pix)
        cout << "  PROBLEM: nside = " << nside << ", pixel = " << pix << endl;
      }
    }
  }

void check_angpixang()
  {
  cout << "testing pix2ang(ang2pix(ptg)) approx ptg" << endl;
  for (int order=0; order<=13; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Base base1 (order,NEST);
    Healpix_Base base2 (order,RING);
    double maxang = base1.max_pixrad();
    for (int m=0; m<nsamples; ++m)
      {
      pointing ptg;
      random_dir (ptg);
      if (v_angle(base1.pix2ang(base1.ang2pix(ptg)),ptg)>maxang)
        cout << "  PROBLEM: order = " << order << ", ptg = " << ptg << endl;
      if (v_angle(base2.pix2ang(base2.ang2pix(ptg)),ptg)>maxang)
        cout << "  PROBLEM: order = " << order << ", ptg = " << ptg << endl;
      }
    }
  for (int nside=3; nside<(1<<13); nside+=nside/2+1)
    {
    cout << "nside = " << nside << endl;
    Healpix_Base base (nside,RING,SET_NSIDE);
    double maxang = base.max_pixrad();
    for (int m=0; m<nsamples; ++m)
      {
      pointing ptg;
      random_dir (ptg);
      if (v_angle(base.pix2ang(base.ang2pix(ptg)),ptg)>maxang)
        cout << "  PROBLEM: nside = " << nside << ", ptg = " << ptg << endl;
      }
    }
  }
void check_angpixang2()
  {
  cout << "testing pix2ang(ang2pix(ptg)) approx ptg" << endl;
  for (int order=0; order<=29; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Base2 base1 (order,NEST);
    Healpix_Base2 base2 (order,RING);
    double maxang = base1.max_pixrad();
    for (int m=0; m<nsamples; ++m)
      {
      pointing ptg;
      random_dir (ptg);
      if (v_angle(base1.pix2ang(base1.ang2pix(ptg)),ptg)>maxang)
        cout << "  PROBLEM: order = " << order << ", ptg = " << ptg << endl;
      if (v_angle(base2.pix2ang(base2.ang2pix(ptg)),ptg)>maxang)
        cout << "  PROBLEM: order = " << order << ", ptg = " << ptg << endl;
      }
    }
  for (int nside=3; nside<(1<<29); nside+=nside/2+1)
    {
    cout << "nside = " << nside << endl;
    Healpix_Base2 base (nside,RING,SET_NSIDE);
    double maxang = base.max_pixrad();
    for (int m=0; m<nsamples; ++m)
      {
      pointing ptg;
      random_dir (ptg);
      if (v_angle(base.pix2ang(base.ang2pix(ptg)),ptg)>maxang)
        cout << "  PROBLEM: nside = " << nside << ", ptg = " << ptg << endl;
      }
    }
  }

void check_neighbors()
  {
  cout << "testing neighbor function" << endl;
  for (int order=0; order<=13; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Base base (order,NEST), base2(order,RING);
    double maxang = 2.01*base.max_pixrad();
    for (int m=0; m<nsamples/10; ++m)
      {
      int pix = int(rng.rand_uni()*base.Npix());
      fix_arr<int,8> nb;
      vec3 pixpt = base.pix2ang(pix);
      base.neighbors(pix,nb);
      sort(&nb[0],&nb[0]+8);
      int check=0;
      for (int n=0; n<8; ++n)
        {
        if (nb[n]<0)
          {
          ++check;
          }
        else
          {
          if (v_angle(base.pix2ang(nb[n]),pixpt)>maxang)
            cout << " PROBLEM: order = " << order << ", pix = " << pix << endl;
          if ((n>0) && (nb[n]==nb[n-1]))
            cout << " PROBLEM: order = " << order << ", pix = " << pix << endl;
          }
        }
      planck_assert((check<=1)||((order==0)&&(check<=2)),"too few neighbors");
      pixpt = base2.pix2ang(pix);
      base2.neighbors(pix,nb);
      for (int n=0; n<8; ++n)
        if ((nb[n]>=0) && (v_angle(base2.pix2ang(nb[n]),pixpt)>maxang))
          cout << "  PROBLEM2: order = " << order << ", pix = " << pix << endl;
      }
    }
  for (int nside=3; nside<(1<<13); nside+=nside/2+1)
    {
    cout << "nside = " << nside << endl;
    Healpix_Base base (nside,RING,SET_NSIDE);
    double maxang = 2.01*base.max_pixrad();
    for (int m=0; m<nsamples/10; ++m)
      {
      int pix = int(rng.rand_uni()*base.Npix());
      fix_arr<int,8> nb;
      vec3 pixpt = base.pix2ang(pix);
      base.neighbors(pix,nb);
      for (int n=0; n<8; ++n)
        if ((nb[n]>=0) && (v_angle(base.pix2ang(nb[n]),pixpt)>maxang))
          cout << "  PROBLEM: nside = " << nside << ", pix = " << pix << endl;
      }
    }
  }
void check_neighbors2()
  {
  cout << "testing neighbor function" << endl;
  for (int order=0; order<=29; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Base2 base (order,NEST), base2(order,RING);
    double maxang = 2.01*base.max_pixrad();
    for (int m=0; m<nsamples/10; ++m)
      {
      int64 pix = int64(rng.rand_uni()*base.Npix());
      fix_arr<int64,8> nb;
      vec3 pixpt = base.pix2ang(pix);
      base.neighbors(pix,nb);
      sort(&nb[0],&nb[0]+8);
      int check=0;
      for (int n=0; n<8; ++n)
        {
        if (nb[n]<0)
          {
          ++check;
          }
        else
          {
          if (v_angle(base.pix2ang(nb[n]),pixpt)>maxang)
            cout << " PROBLEM: order = " << order << ", pix = " << pix << endl;
          if ((n>0) && (nb[n]==nb[n-1]))
            cout << " PROBLEM: order = " << order << ", pix = " << pix << endl;
          }
        }
      planck_assert((check<=1)||((order==0)&&(check<=2)),"too few neighbors");
      pixpt = base2.pix2ang(pix);
      base2.neighbors(pix,nb);
      for (int n=0; n<8; ++n)
        if ((nb[n]>=0) && (v_angle(base2.pix2ang(nb[n]),pixpt)>maxang))
          cout << "  PROBLEM2: order = " << order << ", pix = " << pix << endl;
      }
    }
  for (int nside=3; nside<(1<<29); nside+=nside/2+1)
    {
    cout << "nside = " << nside << endl;
    Healpix_Base2 base (nside,RING,SET_NSIDE);
    double maxang = 2.01*base.max_pixrad();
    for (int m=0; m<nsamples/10; ++m)
      {
      int64 pix = int64(rng.rand_uni()*base.Npix());
      fix_arr<int64,8> nb;
      vec3 pixpt = base.pix2ang(pix);
      base.neighbors(pix,nb);
      for (int n=0; n<8; ++n)
        if ((nb[n]>=0) && (v_angle(base.pix2ang(nb[n]),pixpt)>maxang))
          cout << "  PROBLEM: nside = " << nside << ", pix = " << pix << endl;
      }
    }
  }

void check_swap_scheme()
  {
  cout << "testing whether double swap_scheme() returns the original map"
       << endl << "(for orders 0 to 10)." << endl;
  for (int order=0; order<=10; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Map<uint8> map(order,NEST);
    for (int m=0; m<map.Npix(); ++m) map[m]=uint8(m&0xFF);
    map.swap_scheme();
    map.swap_scheme();
    for (int m=0; m<map.Npix(); ++m)
      if (map[m]!=(m&0xFF))
        cout << "  PROBLEM: order = " << order << ", pix = " << m << endl;
    }
  }

void check_query_disc (Healpix_Ordering_Scheme scheme)
  {
  cout << "testing whether all pixels found by query_disc() really" << endl
       << "lie inside the disk (and vice versa)" << endl;
  cout << "Ordering scheme: " << (scheme==RING ? "RING" : "NEST") << endl;
  for (int order=0; order<=5; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Map <bool> map (order,scheme);
    map.fill(false);
    Healpix_Map<vec3> vmap(order,scheme);
    for (int m=0; m<vmap.Npix(); ++m)
      vmap[m]=vmap.pix2vec(m);
    vector<int> list;
    for (int m=0; m<100000; ++m)
      {
      pointing ptg;
      random_dir (ptg);
      double rad = pi/1 * rng.rand_uni();
      map.query_disc(ptg,rad,list);
      vec3 vptg=ptg;
      double cosrad=cos(rad);
      for (tsize i=0; i<list.size(); ++i)
        map[list[i]] = true;
      for (int i=0; i<map.Npix(); ++i)
        {
        bool inside = dotprod(vmap[i],vptg)>cosrad;
        if (inside^map[i])
          cout << "  PROBLEM: order = " << order << ", ptg = " << ptg << endl;
        }
      for (tsize i=0; i<list.size(); ++i)
        map[list[i]] = false;
      }
    }
  }
void check_query_disc_rangeset (Healpix_Ordering_Scheme scheme)
  {
  cout << "testing whether all pixels found by query_disc() using" << endl
       << "range sets really lie inside the disk (and vice versa)" << endl;
  cout << "Ordering scheme: " << (scheme==RING ? "RING" : "NEST") << endl;
  for (int order=0; order<=5; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Map<bool> map (order,scheme);
    map.fill(false);
    Healpix_Map<vec3> vmap(order,scheme);
    for (int m=0; m<vmap.Npix(); ++m)
      vmap[m]=vmap.pix2vec(m);
    rangeset<int> pixset;
    for (int m=0; m<100000; ++m)
      {
      pointing ptg;
      random_dir (ptg);
      double rad = pi/1 * rng.rand_uni();
      map.query_disc(ptg,rad,pixset);
      vec3 vptg=ptg;
      double cosrad=cos(rad);
      for (rangeset<int>::iterator it=pixset.begin(); it!=pixset.end(); ++it)
        for (int i=it->first; i<it->second; ++i)
          map[i] = true;
      for (int i=0; i<map.Npix(); ++i)
        {
        bool inside = dotprod(vmap[i],vptg)>cosrad;
        if (inside^map[i])
          cout << "  PROBLEM: order = " << order << ", ptg = " << ptg << endl;
        }
      for (rangeset<int>::iterator it=pixset.begin(); it!=pixset.end();++it)
        for (int i=it->first; i<it->second; ++i)
          map[i] = false;
      }
    }
  }

void helper_oop (int order)
  {
  Healpix_Map<double> map (order,RING), map2 (order,NEST), map3 (order,RING);
  for (int m=0; m<map.Npix(); ++m) map[m] = rng.rand_uni()+0.01;
  map2.Import(map);
  map3.Import(map2);
  for (int m=0; m<map.Npix(); ++m)
    if (!approx(map[m],map3[m],1e-12))
      cout << "PROBLEM: order = " << order << endl;
  }
void helper_udgrade (int order, Healpix_Ordering_Scheme s1,
  Healpix_Ordering_Scheme s2)
  {
  Healpix_Map<double> map (order,s1), map2 (order+2,s2), map3 (order, s1);
  for (int m=0; m<map.Npix(); ++m) map[m] = rng.rand_uni()+0.01;
  map2.Import(map);
  map3.Import(map2);
  for (int m=0; m<map.Npix(); ++m)
    if (!approx(map[m],map3[m],1e-12))
      cout << "PROBLEM: order = " << order << endl;
  }
void helper_udgrade2 (int nside)
  {
  Healpix_Map<double> map (nside,RING,SET_NSIDE), map2 (nside*3,RING,SET_NSIDE),
    map3 (nside, RING,SET_NSIDE);
  for (int m=0; m<map.Npix(); ++m) map[m] = rng.rand_uni()+0.01;
  map2.Import(map);
  map3.Import(map2);
  for (int m=0; m<map.Npix(); ++m)
    if (!approx(map[m],map3[m],1e-12))
      cout << "PROBLEM: nside = " << nside << endl;
  }

void check_import()
  {
  cout << "testing out-of-place swapping" << endl;
  for (int order=0; order<=7; ++order)
    {
    cout << "order = " << order << endl;
    helper_oop(order);
    }
  cout << "testing downgrade(upgrade(map)) == map" << endl;
  for (int order=0; order<=7; ++order)
    {
    cout << "order = " << order << endl;
    cout << "RING, RING" << endl;
    helper_udgrade(order,RING,RING);
    cout << "RING, NEST" << endl;
    helper_udgrade(order,RING,NEST);
    cout << "NEST, NEST" << endl;
    helper_udgrade(order,NEST,NEST);
    cout << "NEST, RING" << endl;
    helper_udgrade(order,NEST,RING);
    }
  for (int nside=3; nside<500; nside+=nside/2+1)
    {
    cout << "nside = " << nside << endl;
    helper_udgrade2(nside);
    }
  }

void check_average()
  {
  cout << "testing whether average(map) == average(downgraded map)" << endl;
  for (int order=1; order<=10; ++order)
    {
    cout << "order = " << order << endl;
    Healpix_Map<double> map (order,RING), map2(1,RING);
    for (int m=0; m<map.Npix(); ++m)
      map[m] = rng.rand_uni()+0.01;
    map2.Import(map);
    double avg=map.average(), avg2=map2.average();
    if (!approx(avg,avg2,1e-10))
      cout << "PROBLEM: order = " << order << " " << avg/avg2-1 << endl;
    }
  for (int nside=3; nside<1000; nside += nside/2+1)
    {
    cout << "nside = " << nside << endl;
    Healpix_Map<double> map (nside,RING,SET_NSIDE), map2(1,RING,SET_NSIDE);
    for (int m=0; m<map.Npix(); ++m)
      map[m] = rng.rand_uni()+0.01;
    map2.Import(map);
    double avg=map.average(), avg2=map2.average();
    if (!approx(avg,avg2,1e-10))
      cout << "PROBLEM: nside = " << nside << " " << avg/avg2-1 << endl;
    }
  }

void random_alm (Alm<xcomplex<double> >&almT, Alm<xcomplex<double> >&almG,
  Alm<xcomplex<double> >&almC, int lmax, int mmax)
  {
  almT.Set(lmax,mmax); almG.Set(lmax,mmax); almC.Set(lmax,mmax);

  for (int l=0; l<=lmax; ++l)
    {
    almT(l,0).re=rng.rand_gauss(); almT(l,0).im=0.;
    almG(l,0).re=rng.rand_gauss(); almG(l,0).im=0.;
    almC(l,0).re=rng.rand_gauss(); almC(l,0).im=0.;
    }

  for (int m=1; m<=mmax; ++m)
    for (int l=m; l<=lmax; ++l)
      {
      almT(l,m).re=rng.rand_gauss(); almT(l,m).im=rng.rand_gauss();
      almG(l,m).re=rng.rand_gauss(); almG(l,m).im=rng.rand_gauss();
      almC(l,m).re=rng.rand_gauss(); almC(l,m).im=rng.rand_gauss();
      }
  almG(0,0)=almC(0,0)=almG(1,0)=almC(1,0)=almG(1,1)=almC(1,1)=0;
  }

void random_alm (Alm<xcomplex<double> >&alm, int lmax, int mmax)
  {
  alm.Set(lmax,mmax);

  for (int l=0; l<=lmax; ++l)
    { alm(l,0).re=rng.rand_gauss(); alm(l,0).im=0.; }

  for (int m=1; m<=mmax; ++m)
    for (int l=m; l<=lmax; ++l)
      { alm(l,m).re=rng.rand_gauss(); alm(l,m).im=rng.rand_gauss(); }
  }

void check_alm (const Alm<xcomplex<double> >&oalm,
  const Alm<xcomplex<double> >&alm, double epsilon)
  {
  for (int m=0; m<=alm.Mmax(); ++m)
    for (int l=m; l<=alm.Lmax(); ++l)
      {
      if (!abs_approx(oalm(l,m).re,alm(l,m).re,epsilon))
        cout << "Problemr " << l << " " << m << endl;
      if (!abs_approx(oalm(l,m).im,alm(l,m).im,epsilon))
        cout << "Problemi " << l << " " << m <<  endl;
      }
  }

void check_alm2map2alm (int lmax, int mmax, int nside)
  {
  cout << "testing whether a_lm->map->a_lm returns original a_lm" << endl;
  cout << "lmax=" << lmax <<", mmax=" << mmax << ", nside=" << nside << endl;
  const double epsilon = 1e-8;
  Alm<xcomplex<double> > oalmT(lmax,mmax),almT(lmax,mmax),
    oalmG(lmax,mmax),almG(lmax,mmax),oalmC(lmax,mmax),almC(lmax,mmax);
  Healpix_Map<double> mapT(nside,RING,SET_NSIDE), mapQ(nside,RING,SET_NSIDE),
    mapU(nside,RING,SET_NSIDE);

  random_alm(oalmT,oalmG,oalmC,lmax,mmax);
  alm2map(oalmT,mapT);
  map2alm_iter2(mapT,almT,1e-12,1e-12);
  check_alm (oalmT, almT, epsilon);

  alm2map_spin(oalmG,oalmC,mapQ,mapU,1);
  map2alm_spin_iter2(mapQ,mapU,almG,almC,1,1e-12,1e-12);
  check_alm (oalmG, almG, epsilon);
  check_alm (oalmC, almC, epsilon);

  alm2map_pol(oalmT,oalmG,oalmC,mapT,mapQ,mapU);
  map2alm_pol_iter2(mapT,mapQ,mapU,almT,almG,almC,1e-12,1e-12);
  check_alm (oalmT, almT, epsilon);
  check_alm (oalmG, almG, epsilon);
  check_alm (oalmC, almC, epsilon);
  }

void check_smooth_alm ()
  {
  cout << "testing whether unsmooth(smooth(a_lm)) returns a_lm" << endl;
  const double epsilon = 1e-14;
  const double fwhm = 100.*arcmin2rad;
  const int lmax=300, mmax=300;
  Alm<xcomplex<double> > oalmT(lmax,mmax),almT(lmax,mmax),
    oalmG(lmax,mmax),almG(lmax,mmax),oalmC(lmax,mmax),almC(lmax,mmax);

  random_alm(oalmT,oalmG,oalmC,lmax,mmax);

  almT=oalmT; almG=oalmG; almC=oalmC;
  smoothWithGauss (almT, fwhm);
  smoothWithGauss (almT, -fwhm);
  check_alm (oalmT, almT, epsilon);
  almT=oalmT;
  smoothWithGauss (almT, almG, almC, fwhm);
  smoothWithGauss (almT, almG, almC, -fwhm);
  check_alm (oalmT, almT, epsilon);
  check_alm (oalmG, almG, epsilon);
  check_alm (oalmC, almC, epsilon);
  }

void check_rot_alm ()
  {
  cout << "testing whether rot^-1(rot(a_lm)) returns a_lm" << endl;
  const double epsilon = 2e-13;
  const int lmax=300;
  Alm<xcomplex<double> > oalm(lmax,lmax),alm(lmax,lmax);

  random_alm(oalm,lmax,lmax);

  alm=oalm;
  rotate_alm (alm,3,4,5);
  rotate_alm (alm,-5,-4,-3);
  check_alm (oalm, alm, epsilon);
  }

#define PERF_TEMPLATE1(name,scheme,type,func) \
  { \
  tsize cnt=0; \
  wallTimers.start("timer"); \
  for (int order=0; order<=12; ++order) \
    { \
    Healpix_Base base (order,scheme); \
    for (int pix=0; pix<base.Npix(); ++pix) \
      blub[pix&1023] = base.func(pix); \
    cnt+=base.Npix(); \
    } \
  wallTimers.stop("timer"); \
  cout << name << ": " << cnt/wallTimers.acc("timer")*1e-6 << "MOps/s" << endl;\
  wallTimers.reset("timer"); \
  }

void perftest()
  {
  double dummy=0;
  cout << "Measuring performance of Healpix_Base methods." << endl;
  {
  pointing blub[1024];
  PERF_TEMPLATE1("pix2ang(RING)",RING,pointing,pix2ang)
  PERF_TEMPLATE1("pix2ang(NEST)",NEST,pointing,pix2ang)
  dummy+=blub[234].theta+blub[234].phi;
  }
  {
  vec3 blub[1024];
  PERF_TEMPLATE1("pix2vec(RING)",RING,vec3,pix2vec)
  PERF_TEMPLATE1("pix2vec(NEST)",NEST,vec3,pix2vec)
  dummy+=blub[234].x+blub[234].y+blub[234].z;
  }
  tsize cnt=0;
  wallTimers.start("pix2zphi(RING)");
  for (int order=0; order<=12; ++order)
    {
    Healpix_Base base (order,RING);
    for (int pix=0; pix<base.Npix(); ++pix)
      {
      double z,phi;
      base.pix2zphi(pix,z,phi);
      dummy+=z+phi;
      ++cnt;
      }
    }
  wallTimers.stop("pix2zphi(RING)");
  cout << "pix2zphi(RING): "
       << cnt/wallTimers.acc("pix2zphi(RING)")*1e-6 << "MOps/s" << endl;

  cnt=0;
  wallTimers.start("pix2zphi(NEST)");
  for (int order=0; order<=12; ++order)
    {
    Healpix_Base base (order,NEST);
    for (int pix=0; pix<base.Npix(); ++pix)
      {
      double z,phi;
      base.pix2zphi(pix,z,phi);
      dummy+=z+phi;
      ++cnt;
      }
    }
  wallTimers.stop("pix2zphi(NEST)");
  cout << "pix2zphi(NEST): "
       << cnt/wallTimers.acc("pix2zphi(NEST)")*1e-6 << "MOps/s" << endl;

  cnt=0;
  wallTimers.start("zphi2pix(RING)");
  for (int order=0; order<=12; ++order)
    {
    Healpix_Base base (order,RING);
    for (double z=-1; z<1; z+=0.001)
      for (double phi=0; phi<twopi; phi+=0.001)
        {
        dummy+=base.zphi2pix(z,phi);
        ++cnt;
        }
    }
  wallTimers.stop("zphi2pix(RING)");
  cout << "zphi2pix(RING): "
       << cnt/wallTimers.acc("zphi2pix(RING)")*1e-6 << "MOps/s" << endl;

  cnt=0;
  wallTimers.start("zphi2pix_ring2");
  for (int order=0; order<=12; ++order)
    {
    Healpix_Base2 base (order,RING);
    for (double z=-1; z<1; z+=0.001)
      for (double phi=0; phi<twopi; phi+=0.001)
        {
        dummy+=base.zphi2pix(z,phi);
        ++cnt;
        }
    }
  wallTimers.stop("zphi2pix_ring2");
  cout << "zphi2pix2(RING): "
       << cnt/wallTimers.acc("zphi2pix_ring2")*1e-6 << "MOps/s" << endl;

  cnt=0;
  wallTimers.start("zphi2pix(NEST)");
  for (int order=0; order<=12; ++order)
    {
    Healpix_Base base (order,NEST);
    for (double z=-1; z<1; z+=0.001)
      for (double phi=0; phi<twopi; phi+=0.001)
        {
        dummy+=base.zphi2pix(z,phi);
        ++cnt;
        }
    }
  wallTimers.stop("zphi2pix(NEST)");
  cout << "zphi2pix(NEST): "
       << cnt/wallTimers.acc("zphi2pix(NEST)")*1e-6 << "MOps/s" << endl;

  cnt=0;
  wallTimers.start("ang2pix_ring");
  for (int order=0; order<=12; ++order)
    {
    Healpix_Base base (order,RING);
    for (double theta=0; theta<pi; theta+=0.001)
      for (double phi=0; phi<twopi; phi+=0.001)
        {
        dummy+=base.ang2pix(pointing(theta+1e-15*phi,phi));
        ++cnt;
        }
    }
  wallTimers.stop("ang2pix_ring");
  cout << "ang2pix(RING): "
       << cnt/wallTimers.acc("ang2pix_ring")*1e-6 << "MOps/s" << endl;

  cnt=0;
  wallTimers.start("ang2pix_ring2");
  for (int order=0; order<=12; ++order)
    {
    Healpix_Base2 base (order,RING);
    for (double theta=0; theta<pi; theta+=0.001)
      for (double phi=0; phi<twopi; phi+=0.001)
        {
        dummy+=base.ang2pix(pointing(theta+1e-15*phi,phi));
        ++cnt;
        }
    }
  wallTimers.stop("ang2pix_ring2");
  cout << "ang2pix2(RING): "
       << cnt/wallTimers.acc("ang2pix_ring2")*1e-6 << "MOps/s" << endl;

  cnt=0;
  wallTimers.start("ang2pix_nest");
  for (int order=0; order<=12; ++order)
    {
    Healpix_Base base (order,NEST);
    for (double theta=0; theta<pi; theta+=0.001)
      for (double phi=0; phi<twopi; phi+=0.001)
        {
        dummy+=base.ang2pix(pointing(theta+1e-15*phi,phi));
        ++cnt;
        }
    }
  wallTimers.stop("ang2pix_nest");
  cout << "ang2pix(NEST): "
       << cnt/wallTimers.acc("ang2pix_nest")*1e-6 << "MOps/s" << endl;

  cnt=0;
  wallTimers.start("ring2nest");
  for (int order=0; order<=12; ++order)
    {
    Healpix_Base base (order,RING);
    for (int pix=0; pix<base.Npix(); ++pix)
      dummy+=base.ring2nest(pix);
    cnt+=base.Npix();
    }
  wallTimers.stop("ring2nest");
  cout << "ring2nest: "
       << cnt/wallTimers.acc("ring2nest")*1e-6 << "MOps/s" << endl;

  cnt=0;
  wallTimers.start("nest2ring");
  for (int order=0; order<=12; ++order)
    {
    Healpix_Base base (order,RING);
    for (int pix=0; pix<base.Npix(); ++pix)
      dummy+=base.nest2ring(pix);
    cnt+=base.Npix();
    }
  wallTimers.stop("nest2ring");
  cout << "nest2ring: "
       << cnt/wallTimers.acc("nest2ring")*1e-6 << "MOps/s" << endl;

  {
  wallTimers.start("query_disc");
  Healpix_Base base(4096,RING,SET_NSIDE);
  for (int m=0; m<1000; ++m)
    {
    rangeset<int> pix;
    base.query_disc(pointing(halfpi,0),halfpi/9,pix);
    dummy+=pix.size();
    }
  wallTimers.stop("query_disc");
  cout << "query_disc_rangeset(nside=4096,radius=10deg): "
       << 1000/wallTimers.acc("query_disc")*1e-6 << "MOps/s" << endl;
  }

  if (dummy<0) cout << dummy << endl;
  }

} // unnamed namespace

int main(int argc, const char **argv)
  {
  module_startup ("hpxtest",argc,argv,1,"");
  perftest();
  check_smooth_alm();
  check_rot_alm();
  check_alm2map2alm(620,620,256);
  check_alm2map2alm(620,2,256);
  check_average();
  check_import();
  check_neighbors();
  check_neighbors2();
  check_pixangpix();
  check_pixangpix2();
  check_pixvecpix();
  check_pixvecpix2();
  check_angpixang();
  check_angpixang2();
  check_ringnestring();
  check_ringnestring2();
  check_nestpeanonest();
  check_nestpeanonest2();
  check_swap_scheme();
  check_query_disc(RING);
  check_query_disc(NEST);
  check_query_disc_rangeset(RING);
  check_query_disc_rangeset(NEST);
  }
