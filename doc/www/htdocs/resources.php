<?php require('_header.inc.php'); ?>


<h2>Data in HEALPix format</h2>
<br>
<ul>
<li>
The <a href="http://www.rssd.esa.int/index.php?project=Planck">Planck</a> satellite full sky maps, <a href="http://www.sciops.esa.int/index.php?project=PLANCK&page=Planck_Published_Papers">observed at 30 to 857 GHz with unprecedented accuracy</a>, and their decomposition in cosmological and astrophysical components are available in HEALPix format from the <a href="http://www.sciops.esa.int/index.php?project=planck&page=Planck_Legacy_Archive">Planck Legacy Archive</a>
</li>

<li>
The 
<a href="http://lambda.gsfc.nasa.gov/product/map/current/m_products.cfm" >Wilkinson 
    Microwave Anisotropy Probe (WMAP)</a> satellite data, see
    <a href="http://map.gsfc.nasa.gov/" >WMAP Technical Information</a>
</li>

<li>
The <a href="http://lambda.gsfc.nasa.gov/product/cobe/" >COBE</a>
data, including DMR, DIRBE, and FIRAS
    </li>

<li>
The Haslam and <a href="http://lambda.gsfc.nasa.gov/product/iras/" >IRAS</a> maps
</li>
<!--
<li>
The <a href="http://www.drao-ofr.hia-iha.nrc-cnrc.gc.ca/26msurvey/data.html"
    >DRAO</a> 1.4 GHz polarized maps
</li>
-->

<li>
The Schlegel, Finkbeiner, and Davis
    (<a href="http://lanl.arxiv.org/abs/astro-ph/9710327" >astro-ph/9710327</a>,
     <a href="http://www.journals.uchicago.edu/ApJ/journal/contents/ApJ/v500n2.html" >ApJ. 500, p.525</a>),
    combined
    <a href="http://astro.berkeley.edu/davis/dust/index.html" >COBE/DIRBE-IRAS/SISSA dust emission map</a>
</li>

<li>
An extensive list of 
<a href="http://lambda.gsfc.nasa.gov/product/expt/" >
    past, current and future CMB or SZ experiments and data availability</a> on Lambda web site

</li>

</ul>
</ul>

<h2>Applications related to HEALPix</h2>

<p>

<ul>
<li>
The 
<a href="http://lambda.gsfc.nasa.gov/toolbox/tb_cmbfast_ov.cfm" >
    CMBFAST</a>, <a href="http://camb.info/">CAMB</a> and
<a href="http://lesgourg.web.cern.ch/lesgourg/class.php">CLASS</a>
softwares can be used for the computation of the
theoretical spectra of CMB anisotropy. The HEALPix synfast
program reads in the output of these applications to allow one to
generate random realizations of the observable CMB sky
</li>
<li>
The 
<a href="http://cosmologist.info/lenspix" >
    LensPix</a>
software can be used to simulate lensed polarized CMB maps in HEALPix format using MPI
</li>

<li>
The java based <a href="http://aladin.u-strasbg.fr/">Aladin Sky Atlas</a> also supports HEALPix maps in FITS format
</li>

<li>
<a href="https://github.com/healpy/healpy">Healpy</a>: 
python wrapper to HEALPix (now included in HEALPix package)
  </li>

<li>
<a href="http://prof.planck.fr/article141.html" > PolSPICE</a>: 
Temperature+Polarization power spectrum extraction tool dealing with the effects the
cut-sky, beam smoothing, noise contamination, ...
    </li>

<li>
<a href="http://www2.iap.fr/users/sousbie/web/html/indexd41d.html?"> DisPerSE</a>: Automatic identification of persistent structures in 2D & 3D.
</li>

<li>
Wavelets on the Sphere: 
<a href="http://jstarck.free.fr/isap.html">iSAP</a>, 
<a href="http://www.s2let.org/">S2LET</a>, 
<a href="http://www.apc.univ-paris7.fr/Downloads/adamis/spherelab/spherelab.html">SphereLab</a>
</li>

<li>
Take a look at a very useful compilation of 
<a href="http://www.mpa-garching.mpg.de/~banday/CMB.html" >
    CMB Resources</a>
by Anthony J. Banday
</li>

</ul>
</p>

<h2>HEALPix in the literature</h2>

<p>
Published articles using HEALPix: 
<ul>
<li>
First HEALPix paper: <a href="http://adsabs.harvard.edu/cgi-bin/nph-bib_query?bibcode=2005ApJ...622..759G&amp;db_key=AST&amp;high=41069202cf02947">K.M. G&oacute;rski et al., 2005, ApJ, 622, p759</a>
</li>
<li>HEALPix 
on <a href="http://inspirebeta.net/search?ln=en&p=FIND+C+ASTRO-PH%2F9812350+or+refersto%3Arecid%3A659804+or+fulltext%3Ahealpix&f=&action_search=Search&sf=&so=d&rm=&rg=25&sc=0&of=hb">INSPIRE</a>,
																				   <a href="http://adsabs.harvard.edu/cgi-bin/nph-ref_query?bibcode=2005ApJ...622..759G&amp;refs=CITATIONS&amp;db_key=AST">ADS</a>,
																				   <a href="http://scholar.google.com/scholar?q=healpix&btnG=&hl=en&as_sdt=0%2C5">Google Scholar</a>
</li>
</ul>
</p>
<p>
Misc
<ul>
<li>
<a href="http://en.wikipedia.org/wiki/Healpix" >Wikipedia Page on HEALPix</a>
</li>
<li>
	HEALPix on <a href="https://www.ohloh.net/p/healpix">Ohloh</a> and
<a href="http://www.digplanet.com/wiki/HEALPix#">digplanet</a>
</li>
</ul>
</p>




<?php require('_footer.inc.php'); ?>
