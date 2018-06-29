/*******************************************************************************
 * @author  Joseph Kamel 
 * @email   josephekamel@gmail.com
 * @date    11/04/2018
 * @version 1.0
 *
 * SCA (Secure Cooperative Autonomous systems)
 * Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
 * All rights reserved.
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>     /* atof */
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <vector>

#include "AggrigationApp.h"

using namespace std;
using namespace boost;

AggrigationApp::AggrigationApp(const char* name, int version, double deltaTrustTime, int maxBsmTrustNum) :
        MDApplication(name) {
    this->version = version;
    this->deltaTrustTime = deltaTrustTime;
    this->maxBsmTrustNum = maxBsmTrustNum;
}

std::tuple<double, MDReport> AggrigationApp::CheckNodeForReport(int myId,
        BasicSafetyMessage bsm, BsmCheck bsmCheck, NodeTable detectedNodes) {

    bool checkFailed = false;
    MDReport mbReport;

    double temp = 0;
    minFactor = 1;

    prntApp.incAll(bsm.getSenderMbTypeStr());
    prntAppInst.incAll(bsm.getSenderMbTypeStr());

    int senderId = bsm.getSenderAddress();

    MDMHistory mdmHist = detectedNodes.getMDMHistory(senderId);
    NodeHistory nodeHist = detectedNodes.getNodeHistory(senderId);

    BsmCheck bsmCheckList[maxBsmTrustNum];
    int bsmCheckListSize = 0;

    for (int var = 0; var < nodeHist.getBSMNum(); ++var) {
        if (bsmCheckListSize < maxBsmTrustNum) {
            if (mdmLib.calculateDeltaTime(bsm,
                    nodeHist.getBSM(var))<deltaTrustTime) {
                bsmCheckList[bsmCheckListSize] = mdmHist.getBsmCheck(var,
                        version);
                bsmCheckListSize++;
            }
        }
    }

    double factorList[bsmCheckListSize];

    //std::cout<< "RangePlausibility" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getRangePlausibility();
    }

    temp = AggregateFactorsListDouble(bsmCheck.getRangePlausibility(), factorList,
            bsmCheckListSize);
    if(temp<minFactor){
        temp = minFactor;
    }
    if (temp<Threshold) {
        checkFailed = true;
        prntApp.incFlags("RangePlausibility", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("RangePlausibility", bsm.getSenderMbTypeStr());
    }

    //std::cout<< "PositionConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionConsistancy();
    }
    temp = AggregateFactorsListDouble(bsmCheck.getPositionConsistancy(), factorList,
            bsmCheckListSize);
    if(temp<minFactor){
        temp = minFactor;
    }
    if (temp<Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionConsistancy", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("PositionConsistancy", bsm.getSenderMbTypeStr());
    }

    //std::cout<< "PositionSpeedConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionSpeedConsistancy();
    }
    temp = AggregateFactorsListDouble(bsmCheck.getPositionSpeedConsistancy(), factorList,
            bsmCheckListSize);
    if(temp<minFactor){
        temp = minFactor;
    }
    if (temp<Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionSpeedConsistancy", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("PositionSpeedConsistancy", bsm.getSenderMbTypeStr());
    }

    //std::cout<< "SpeedConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getSpeedConsistancy();
    }
    temp = AggregateFactorsListDouble(bsmCheck.getSpeedConsistancy(), factorList,
            bsmCheckListSize);
    if(temp<minFactor){
        temp = minFactor;
    }
    if (temp<Threshold) {
        checkFailed = true;
        prntApp.incFlags("SpeedConsistancy", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("SpeedConsistancy", bsm.getSenderMbTypeStr());
    }

    //std::cout<< "SpeedPlausibility" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getSpeedPlausibility();
    }
    temp = AggregateFactorsListDouble(bsmCheck.getSpeedPlausibility(), factorList,
            bsmCheckListSize);
    if(temp<minFactor){
        temp = minFactor;
    }
    if (temp<Threshold) {
        checkFailed = true;
        prntApp.incFlags("SpeedPlausibility", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("SpeedPlausibility", bsm.getSenderMbTypeStr());
    }

    //std::cout<< "PositionPlausibility" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionPlausibility();
    }
    temp = AggregateFactorsListDouble(bsmCheck.getPositionPlausibility(), factorList,
            bsmCheckListSize);
    if(temp<minFactor){
        temp = minFactor;
    }
    if (temp<Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionPlausibility", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("PositionPlausibility", bsm.getSenderMbTypeStr());
    }

    //std::cout<< "BeaconFrequency" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getBeaconFrequency();
    }
    temp = AggregateFactorsListDouble(bsmCheck.getBeaconFrequency(), factorList,
            bsmCheckListSize);
    if(temp<minFactor){
        temp = minFactor;
    }
    if (temp<Threshold) {
        checkFailed = true;
        prntApp.incFlags("BeaconFrequency", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("BeaconFrequency", bsm.getSenderMbTypeStr());
    }

    //std::cout<< "SuddenAppearence" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getSuddenAppearence();
    }
    temp = AggregateFactorsListDouble(bsmCheck.getSuddenAppearence(), factorList,
            bsmCheckListSize);
    if(temp<minFactor){
   //     temp = minFactor;
    }
    if (temp<Threshold) {
        prntApp.incFlags("SuddenAppearence", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("SuddenAppearence", bsm.getSenderMbTypeStr());
    }

    //std::cout<< "PositionHeadingConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionHeadingConsistancy();
    }
    temp = AggregateFactorsListDouble(bsmCheck.getPositionHeadingConsistancy(), factorList,
            bsmCheckListSize);
    if(temp<minFactor){
        temp = minFactor;
    }
    if (temp<Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionHeadingConsistancy", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("PositionHeadingConsistancy", bsm.getSenderMbTypeStr());
    }

    InterTest inter = bsmCheck.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {
        double curInferFactor = inter.getInterValue(var);

        for (int i = 0; i < bsmCheckListSize; ++i) {
            double IdIndex = bsmCheckList[i].getIntersection().getIdIndex(
                    inter.getInterId(var));
            if (IdIndex != -1) {
                factorList[i] = bsmCheckList[i].getIntersection().getInterValue(
                        IdIndex);
            } else {
                factorList[i] = 1;
            }
        }
        //std::cout<< "Intersection" << '\n';
        temp = AggregateFactorsListDouble(curInferFactor, factorList,
                bsmCheckListSize);
        if(temp<minFactor){
            temp = minFactor;
        }
        if (temp<Threshold) {
            checkFailed = true;
            prntApp.incFlags("Intersection", bsm.getSenderMbTypeStr());
            prntAppInst.incFlags("Intersection", bsm.getSenderMbTypeStr());
        }
    }

    if (checkFailed) {
        mbReport.setGenerationTime(simTime().dbl());
        mbReport.setSenderId(myId);
        mbReport.setReportedId(senderId);
        mbReport.setMbType(bsm.getSenderMbTypeStr());
        mbReport.setAttackType(bsm.getSenderAttackTypeStr());
//        SendReport(mbReport);
        prntApp.incCumulFlags(bsm.getSenderMbTypeStr());
        prntAppInst.incCumulFlags(bsm.getSenderMbTypeStr());

        bsmCheck.setReported(true);
    }
    return std::make_tuple(checkFailed, mbReport);
}


double AggrigationApp::AggregateFactorsListDouble(double curFactor, double *factorList,
        int factorListSize) {
    if (version == 1) {
        double averageFactor = curFactor;
        for (int var = 0; var < factorListSize; ++var) {
            averageFactor = averageFactor + factorList[var];
        }
        averageFactor = averageFactor / (factorListSize + 1);

        return averageFactor;
    } else {
        if (curFactor <= 0) {
            return 0;
        } else {
            double averageFactor = curFactor;
            for (int var = 0; var < factorListSize; ++var) {
                averageFactor = averageFactor + factorList[var];
            }
            averageFactor = averageFactor / (factorListSize + 1);
            return averageFactor;
        }
    }
}

//best rate / faulty
//bool AggrigationApp::AggregateFactorsList(double curFactor, double *factorList,
//        int factorListSize) {
//    if (version == 1) {
//        double averageFactor = curFactor;
//        for (int var = 0; var < factorListSize; ++var) {
//            averageFactor = averageFactor + factorList[var];
//        }
//        averageFactor = averageFactor / (factorListSize + 1);
//        if ((averageFactor) <= 0.5) {
//            return true;
//        } else {
//            return false;
//        }
//    } else {
//        if (curFactor <= 0) {
//            return true;
//        } else {
//            double averageFactor = curFactor;
//            for (int var = 0; var < factorListSize; ++var) {
//                averageFactor = averageFactor + factorList[var];
//            }
//            averageFactor = averageFactor / (factorListSize + 1);
//
//            if ((averageFactor) <= 0.5) {
//                return true;
//            } else {
//                return false;
//            }
//        }
//    }
//}


double AggrigationApp::getMinFactor(){
    return minFactor;
}
