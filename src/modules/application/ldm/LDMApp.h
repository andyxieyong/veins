//
// Copyright (C) 2011 David Eckhoff <eckhoff@cs.fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#ifndef LDMAPP_H_
#define LDMAPP_H_

#include "BaseWaveApplLayer.h"
#include <map>
#include <BaseApplLayer.h>
#include <Consts80211p.h>
#include <WaveShortMessage_m.h>
#include "Coord.h"
#include "base/connectionManager/ChannelAccess.h"
#include <WaveAppToMac1609_4Interface.h>
#include "modules/mobility/traci/TraCIMobility.h"

using Veins::TraCIMobility;
using Veins::AnnotationManager;

#ifndef DBG
#define DBG EV
//#define DBG (ev.isDisabled()||!debug) ? ev : ev << "[" << simTime().raw() << "] " << getParentModule()->getFullPath() << " "
#endif


/**
 * @brief description of a single local dynamic map entry.
 * @author Rens van der Heijden
 */
struct LDMEntry {
        simtime_t time;  //timestamp for the most recent update
        Coord pos; //most recent position
        int speed; //most recent speed value
        // more data elements can be put here.
};

/**
 * @brief LDM that uses BaseWaveApplLayer
 *
 * @author Rens van der Heijden
 *
 * @ingroup ldm
 *
 * @see TraCIDemo11p
 * @see BaseWaveApplLayer
 * @see Mac1609_4
 * @see PhyLayer80211p
 * @see Decider80211p
 */
class LDMApp : public BaseWaveApplLayer {

	public:
		~LDMApp();
		virtual void initialize(int stage);
		virtual void finish();

	protected:
		TraCIMobility* traci;
		AnnotationManager* annotations;
		simtime_t lastDroveAt;
		bool sentMessage;
		bool isParking;
		bool sendWhileParking;
                //maps addresses to their respective LDM entries.
                std::map<int,LDMEntry*> ldm;


	protected:
                //storage method -- override to add tests etc.
		virtual void storeInLDM(const int sender, LDMEntry* data);
                //retrieval method
                virtual const LDMEntry* fetchFromLDM(const int sender) const;

                //reception method for beacons -- override when message type is different
		virtual void onBeacon(WaveShortMessage * wsm) override;
                //overrides BaseWaveApplLayer::prepareWSM -- not yet implemented.
                //virtual WaveShortMessage* prepareWSM(std::string name, int dataLengthBits, t_channel channle, int priority, int rcvId, int serial=0) override;
                //overrides BaseWaveApplLayer::handlePositionUpdate
                virtual void handlePositionUpdate(cObject* obj) override;
                //overrides BaseWaveApplLayer::handleLowerMsg
                virtual void handleLowerMsg(cMessage* msg) override;
                //overrides BaseWaveApplLayer::handleSelfMsg
                virtual void handleSelfMsg(cMessage* msg) override;
                //
                virtual void onData(WaveShortMessage* wsm) override;

                // from TraCIDemo11p:
                //wrapper that sends a message of type "data":
		void sendMessage(std::string blockedRoadId);
		virtual void sendWSM(WaveShortMessage* wsm);
};

#endif /* LDMAPP_H_ */
