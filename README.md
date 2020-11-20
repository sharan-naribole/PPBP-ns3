# Poisson Pareto Burst Process (PPBP) ns-3 traffic generator

## Motivation
This repository is an extension to the [PPBP ns-3 generator](http://perso.ens-lyon.fr/thomas.begin/NS3-PPBP.zip) designed by Doreid Ammar. Unfortunately, their version is not compatible with newer releases of ns-3(ns-3.15 and later). This is mainly because of the shift in the random variable creation in the newer versions of ns-3. In ns-3.14 and earlier, ns-3 simulations used a different wrapper class called ns3::RandomVariable. As of ns-3.15, this class has been replaced by ns3::RandomVariableStream; the underlying pseudo-random number generator has not changed. Most of the ns-3 random variable classes have changed from ExponentialVariable to ExponentialRandomVariable etc. including modified attributes.

## Poisson Pareto Burst Process (PPBP) Primer

The key principles of Poisson Pareto burst process are as follows:

- Burst arrivals follow a Poisson distribution

- The burst lengths follow a Pareto distribution. For large burst lengths, overlapping traffic aggregate to form a long-range dependent traffic model.

- Each burst is modeled by  a constant bit rate.

## Instructions

- ns-3 installation and tutorial https://www.nsnam.org/docs/tutorial/html/index.html

- Copy [PPBP-application.cc](https://github.com/sharan-naribole/PPBP-ns3/blob/master/PPBP-application.cc) and [PPBP-application.h](https://github.com/sharan-naribole/PPBP-ns3/blob/master/PPBP-application.cc) to /src/applications/model directory.

- Copy [PPBP-helper.cc](https://github.com/sharan-naribole/PPBP-ns3/blob/master/PPBP-helper.cc) and [PPBP-helper.h](https://github.com/sharan-naribole/PPBP-ns3/blob/master/PPBP-helper.h) to src/applications/helper directory.

- The public header and source code files for your new module should be specified in the wscript file by modifying it with your text editor. For this, open the wscript in the src/applications directory to include the above files. For an example, go through Step 3 - Declaring Source Files of [Adding a New Module to ns-3](https://www.nsnam.org/docs/manual/html/new-modules.html).

- Build ns-3 by moving to your main ns-3-dev folder and running ``` ./waf build```

- Copy the example file [PPBP-application-test.cc](https://github.com/sharan-naribole/PPBP-ns3/blob/master/PPBP-application-test.cc) to your scratch directory and run ``` ./waf; ./waf --run scratch/PPBP-application-test```

## References

- Doreid Ammar's [PPBP traffic generator](http://perso.ens-lyon.fr/thomas.begin/NS3-PPBP.zip) for older versions of ns-3.

- D. Ammar, T. Begin and I. Guerin Lassous, "A new tool for generating realistic Internet traffic in NS-3", Proc. of International ICST Conference on Simulation Tools and Techniques (SIMUTools), Barcelona, Spain, March 21-25, 2011, Poster.
   
- M. Zukerman, T. D. Neame and R. G. Addie, "Internet Traffic Modeling and Future Technology Implications", Proc. of IEEE INFOCOM 2003, San Francisco, USA, April 2003.
