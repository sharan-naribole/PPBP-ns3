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

// This script configures 1 AP node and N station nodes connected both via WiFi
// and CSMA. The AP transmits poll request to the station nodes
// using CSMA in sequential manner.
// On receiving a poll request, a station node determines randomly if it
// wants to send a packet or not. If the client has baclogged data, it transmits
// over WiFi. If the AP does not hear any packet within
// PIFS duration (25 microseconds but configurable),
// then it transmits the poll request to next client in schedule. Otherwise, it
// waits for the packet reception to complete and only then transmit the next
// poll request.

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

void TxTrace(std::string context, Ptr<const Packet> packet)
{
  std::cout << "Packet transmitted by"
  << context << " Time: "
  << Simulator::Now().GetSeconds()
  << std::endl;

  //std::cout << "TX p: " << *packet << std::endl;
}

void ReceivePacket (Ptr<Socket> socket)
{
  while (socket->Recv ())
    {
      std::cout << "Received one packet at"
      << Simulator::Now().GetSeconds() << std::endl;
    }
}

int
main (int argc, char *argv[])
{

  uint32_t nPackets = 10;

  CommandLine cmd;
  cmd.AddValue("nPackets","Number of packets to echo",nPackets);
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

  PPBPHelper ppbp = PPBPHelper ("ns3::TcpSocketFactory",
                       InetSocketAddress (interfaces.GetAddress (1),socketPort));
  ApplicationContainer apps = ppbp.Install (nodes.Get (0));
  apps.Start (Seconds (0));
  apps.Stop (Seconds (1.0));

  Ptr<Socket> recvSink = Socket::CreateSocket (nodes.Get (1), TcpSocketFactory::GetTypeId ());
  InetSocketAddress local = InetSocketAddress (interfaces.GetAddress (1), socketPort);
  recvSink->Bind (local);
  recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

  Simulator::Stop (Seconds (1.0));

  Config::Connect("/NodeList/*/ApplicationList/*/Tx", MakeCallback (&TxTrace));


  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
