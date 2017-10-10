/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* This script configures two nodes connected vis CSMA channel. One of the
*  nodes transmits packets via PPBP application to the other node.
*  Command line parameters are the simulationTime and verbose for logging.
*  Example run: Copy to scratch folder and run
*  ./waf --run "scratch/PPBP-application-test --simulationTime=10.0 --verbose=true"
*  Author: Sharan Naribole <nsharan@rice.edu>
*/

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"

using namespace ns3;
bool verbose = true;

NS_LOG_COMPONENT_DEFINE ("PPBPExample");

void TxTrace(std::string context, Ptr<const Packet> packet)
{
  if(verbose)
  {
    NS_LOG_UNCOND("Packet transmitted by "
    << context << " Time: "
    << Simulator::Now().GetSeconds());
  }
}

void ReceivePacket (Ptr<Socket> socket)
{
  while (socket->Recv ())
    {
      if(verbose)
      {
        NS_LOG_UNCOND("Received one packet at "
        << Simulator::Now().GetSeconds());
      }
    }
}

int
main (int argc, char *argv[])
{

  double simulationTime = 5; //seconds

  CommandLine cmd;
  cmd.AddValue("simulationTime","Simulation time",simulationTime);
  cmd.AddValue("verbose","Output transmission and reception timestamps",verbose);
  cmd.Parse (argc, argv);

  Time::SetResolution (Time::NS);

  NS_LOG_INFO("Creating Topology");
  NodeContainer nodes;
  nodes.Create (2);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer devices = csma.Install(nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);


  uint32_t socketPort = 9;

  PPBPHelper ppbp = PPBPHelper ("ns3::UdpSocketFactory",
                       InetSocketAddress (interfaces.GetAddress (1),socketPort));
  ApplicationContainer apps = ppbp.Install (nodes.Get (0));
  apps.Start (Seconds (0));
  apps.Stop (Seconds (simulationTime));

  Ptr<Socket> recvSink = Socket::CreateSocket (nodes.Get (1), UdpSocketFactory::GetTypeId ());
  InetSocketAddress local = InetSocketAddress (interfaces.GetAddress (1), socketPort);
  recvSink->Bind (local);
  recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

  Simulator::Stop (Seconds (simulationTime));

  Config::Connect("/NodeList/*/ApplicationList/*/Tx", MakeCallback (&TxTrace));


  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
