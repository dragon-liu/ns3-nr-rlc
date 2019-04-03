#ifndef NR_MAC_SAP_H
#define NR_MAC_SAP_H

#include <ns3/packet.h>

namespace ns3 {

/**
 * Service Access Point (SAP) offered by the MAC to the RLC
 *
 */
class NrMacSapProvider
{
public:
  virtual ~NrMacSapProvider ();

  /**
   * Parameters for NrMacSapProvider::TransmitPdu
   *
   */
  struct TransmitPduParameters
  {
    Ptr<Packet> pdu; /**< the RLC PDU */
    uint16_t rnti; /**< the C-RNTI identifying the UE */
    uint8_t lcid; /**< the logical channel id corresponding to the sending RLC instance */
    uint8_t
        layer; /**< the layer value that was passed by the MAC in the call to NotifyTxOpportunity that generated this PDU */
    uint8_t
        harqProcessId; /**< the HARQ process id that was passed by the MAC in the call to NotifyTxOpportunity that generated this PDU */
  };

  /**
   * send an RLC PDU to the MAC for transmission. This method is to be
   * called as a response to NrMacSapUser::NotifyTxOpportunity
   */
  virtual void TransmitPdu (TransmitPduParameters params) = 0;

  /**
   * Parameters for NrMacSapProvider::ReportBufferStatus
   *
   * \param params
   */
  struct ReportBufferStatusParameters
  {
    uint16_t rnti; /**< the C-RNTI identifying the UE */
    uint8_t lcid; /**< the logical channel id corresponding to the sending RLC instance */
    uint32_t txQueueSize; /**< the current size of the RLC transmission queue */
    uint16_t txQueueHolDelay; /**< the Head Of Line delay of the transmission queue */
    uint32_t retxQueueSize; /**<  the current size of the RLC retransmission queue in bytes */
    uint16_t retxQueueHolDelay; /**<  the Head Of Line delay of the retransmission queue */
    uint16_t statusPduSize; /**< the current size of the pending STATUS RLC  PDU message in bytes */
  };

  /**
   * Report the RLC buffer status to the MAC
   *
   * \param params
   */
  virtual void ReportBufferStatus (ReportBufferStatusParameters params) = 0;
};

/**
 * Service Access Point (SAP) offered by the MAC to the RLC
 * See Femto Forum MAC Scheduler Interface Specification v 1.11, Figure 1
 *
 * This is the MAC SAP User, i.e., the part of the SAP that contains the RLC methods called by the MAC
 */
class NrMacSapUser
{
public:
  virtual ~NrMacSapUser ();
  /**
   * Called by the MAC to notify the RLC that the scheduler granted a
   * transmission opportunity to this RLC instance.
   *
   * \param bytes the number of bytes to transmit
   * \param layer the layer of transmission (MIMO)
   */
  virtual void NotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId) = 0;

  /**
   * Called by the MAC to notify the RLC that an HARQ process related
   * to this RLC instance has failed
   *
   */
  virtual void NotifyHarqDeliveryFailure () = 0;

  virtual void NotifyDlHarqDeliveryFailure (uint8_t harqId);

  virtual void NotifyUlHarqDeliveryFailure (uint8_t harqId);

  /**
   * Called by the MAC to notify the RLC of the reception of a new PDU
   *
   * \param p
   */
  virtual void ReceivePdu (Ptr<Packet> p) = 0;
};

///////////////////////////////////////

template <class C>
class EnbMacMemberNrMacSapProvider : public NrMacSapProvider
{
public:
  EnbMacMemberNrMacSapProvider (C *mac);

  // inherited from NrMacSapProvider
  virtual void TransmitPdu (TransmitPduParameters params);
  virtual void ReportBufferStatus (ReportBufferStatusParameters params);

private:
  C *m_mac;
};
template <class C>
EnbMacMemberNrMacSapProvider<C>::EnbMacMemberNrMacSapProvider (C *mac) : m_mac (mac)
{
}

template <class C>
void
EnbMacMemberNrMacSapProvider<C>::TransmitPdu (TransmitPduParameters params)
{
  m_mac->DoTransmitPdu (params);
}

template <class C>
void
EnbMacMemberNrMacSapProvider<C>::ReportBufferStatus (ReportBufferStatusParameters params)
{
  m_mac->DoReportBufferStatus (params);
}
} // namespace ns3

#endif // NR_MAC_SAP_H
