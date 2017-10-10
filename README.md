# Poisson Pareto Burst Process (PPBP) ns-3 traffic generator

## Motivation
This repository is an extension to the PPBP ns-3 generator designed by Doreid Ammar available at http://perso.ens-lyon.fr/thomas.begin/NS3-PPBP.zip. Unfortunately, their version is not compatible with newer releases of ns-3(ns-3.15 and later). This is mainly because of the shift in the random variable creation in the newer versions of ns-3. In ns-3.14 and earlier, ns-3 simulations used a different wrapper class called ns3::RandomVariable. As of ns-3.15, this class has been replaced by ns3::RandomVariableStream; the underlying pseudo-random number generator has not changed. Most of the ns-3 random variable classes have changed from ExponentialVariable to ExponentialRandomVariable etc. including modified attributes.

## Poisson Pareto Burst Process (PPBP) Primer

The key principles of Poisson Pareto burst process are as follows:

- Burst arrivals follow a Poisson distribution

- The burst lengths follow a Pareto distribution. For large burst lengths, overlapping traffic aggregate to form a long-range dependent traffic model.

- Each burst is modeled by  a constant bit rate.

