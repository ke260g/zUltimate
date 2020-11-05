[TOC]
/proc/net/netstat
1. netstat -s
    1. 检查 /proc/net/snmp    初步诊断出问题
    2. 检查 /proc/net/netstat 初步诊断出问题
```sh
cat /proc/net/netstat | grep IpExt: | head -1 | sed -e 's| |\n|g' > IpExt.param
cat /proc/net/netstat | grep IpExt: | tail -1 | sed -e 's| |\n|g' > IpExt.value
sdiff IpExt.param IpExt.value

cat /proc/net/netstat | grep TcpExt: | head -1 | sed -e 's| |\n|g' > TcpExt.param
cat /proc/net/netstat | grep TcpExt: | tail -1 | sed -e 's| |\n|g' > TcpExt.value
sdiff TcpExt.param TcpExt.value
```

# TcpExt
## SyncookiesSent
## SyncookiesRecv
## SyncookiesFailed
## EmbryonicRsts
## PruneCalled
## RcvPruned
## OfoPruned
## OutOfWindowIcmps
## LockDroppedIcmps
## ArpFilter
## TW
## TWRecycled
## TWKilled
## PAWSActive
## PAWSEstab
## DelayedACKs
## DelayedACKLocked
## DelayedACKLost
## ListenOverflows
## ListenDrops
## TCPHPHits
## TCPPureAcks
## TCPHPAcks
## TCPRenoRecovery
## TCPSackRecovery
## TCPSACKReneging
## TCPSACKReorder
## TCPRenoReorder
## TCPTSReorder
## TCPFullUndo
## TCPPartialUndo
## TCPDSACKUndo
## TCPLossUndo
## TCPLostRetransmit
## TCPRenoFailures
## TCPSackFailures
## TCPLossFailures
## TCPFastRetrans
## TCPSlowStartRetrans
## TCPTimeouts
## TCPLossProbes
## TCPLossProbeRecovery
## TCPRenoRecoveryFail
## TCPSackRecoveryFail
## TCPRcvCollapsed
## TCPBacklogCoalesce
## TCPDSACKOldSent
## TCPDSACKOfoSent
## TCPDSACKRecv
## TCPDSACKOfoRecv
## TCPAbortOnData
## TCPAbortOnClose
## TCPAbortOnMemory
## TCPAbortOnTimeout
## TCPAbortOnLinger
## TCPAbortFailed
## TCPMemoryPressures
## TCPMemoryPressuresChrono
## TCPSACKDiscard
## TCPDSACKIgnoredOld
## TCPDSACKIgnoredNoUndo
## TCPSpuriousRTOs
## TCPMD5NotFound
## TCPMD5Unexpected
## TCPMD5Failure
## TCPSackShifted
## TCPSackMerged
## TCPSackShiftFallback
## TCPBacklogDrop
## PFMemallocDrop
## TCPMinTTLDrop
## TCPDeferAcceptDrop
## IPReversePathFilter
## TCPTimeWaitOverflow
## TCPReqQFullDoCookies
## TCPReqQFullDrop
## TCPRetransFail
## TCPRcvCoalesce
## TCPOFOQueue
## TCPOFODrop
## TCPOFOMerge
## TCPChallengeACK
## TCPSYNChallenge
## TCPFastOpenActive
## TCPFastOpenActiveFail
## TCPFastOpenPassive
## TCPFastOpenPassiveFail
## TCPFastOpenListenOverflow
## TCPFastOpenCookieReqd
## TCPFastOpenBlackhole
## TCPSpuriousRtxHostQueues
## BusyPollRxPackets
## TCPAutoCorking
## TCPFromZeroWindowAdv
## TCPToZeroWindowAdv
## TCPWantZeroWindowAdv
## TCPSynRetrans
## TCPOrigDataSent
## TCPHystartTrainDetect
## TCPHystartTrainCwnd
## TCPHystartDelayDetect
## TCPHystartDelayCwnd
## TCPACKSkippedSynRecv
## TCPACKSkippedPAWS
## TCPACKSkippedSeq
## TCPACKSkippedFinWait2
## TCPACKSkippedTimeWait
## TCPACKSkippedChallenge
## TCPWinProbe
## TCPKeepAlive
## TCPMTUPFail
## TCPMTUPSuccess
## TCPDelivered
## TCPDeliveredCE
## TCPAckCompressed
## TCPZeroWindowDrop
## TCPRcvQDrop
## TCPWqueueTooBig
## TCPFastOpenPassiveAltKey

# IpExt
## InNoRoutes
## InTruncatedPkts
## InMcastPkts
## OutMcastPkts
## InBcastPkts
## OutBcastPkts
## InOctets
## OutOctets
## InMcastOctets
## OutMcastOctets
## InBcastOctets
## OutBcastOctets
## InCsumErrors
## InNoECTPkts
## InECT1Pkts
## InECT0Pkts
## InCEPkts
## ReasmOverlaps