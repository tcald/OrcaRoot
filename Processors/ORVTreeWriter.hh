// ORVTreeWriter.hh

#ifndef _ORVTreeWriter_hh_
#define _ORVTreeWriter_hh_

#include <string>
#ifndef ROOT_TTree
  #ifndef __CINT__
#include "TTree.h"
  #else
class TTree;
  #endif
#endif
#ifndef _ORDataProcessor_hh_
#include "ORDataProcessor.hh"
#endif


class ORVTreeWriter : public ORDataProcessor
{
  public:
    enum EFillMethod { kFillAfterProcessDataRecord, kFillBeforeProcessDataRecord };

  public:
    ORVTreeWriter(ORVDataDecoder* decoder, std::string treeName = "");
    virtual ~ORVTreeWriter() {}

    virtual EReturnCode StartRun();
    virtual EReturnCode ProcessDataRecord(UInt_t* record);
    virtual EReturnCode EndRun();
    virtual void SetTreeName(std::string treeName) { fTreeName = treeName; }

    // The following functions set the automatic filling behavior of the
    // ORVTreeWriter. Note that ORVTreeWriters auto-fill the tree with
    // period 1 and method kFillAfterProcessDataRecord by default.

    // In some cases, several processors might be writing to the same tree.
    // However, only one processor should do the actual filling of the
    // tree. This behavior may be selected by calling
    // SetThisProcessorAutoFillsTree(true) for the processor in charge of tree
    // filling, and SetThisProcessorAutoFillsTree(false) for all other
    // processors.
    virtual void SetThisProcessorAutoFillsTree(bool fillTree = true);

    // Some devices send their information serially in a sequence of data
    // records. For example, a collection of 4 ADCs might send 4 contiguous
    // records, and you only want to fill the tree once every 4 times a
    // data record is received. This is achieved by SetFillPeriod(nRecords)
    // (e.g. nRecords = 4 for this example).
    virtual void SetFillPeriod(int nRecords) { fFillPeriod = nRecords; }

    // For a simple tree filled by a single processor, filling the tree
    // after each data record is processed is sufficient. In such cases,
    // users should call SetFillMethod(kFillAfterProcessDataRecord). This
    // is the default
    //
    // However, say you have a series of processors that are filling
    // the same tree (or an object to be saved to a tree), where the
    // arrival of one data record type (for example a GTID) marks when
    // a new row (e.g. event) begins.  In this case, you want the
    // filling of the tree to be postponed until right before the
    // processing of the next data record of that type. In such cases,
    // SetFillMethod(kFillBeforeProcessDataRecord) should be used.
    virtual void SetFillMethod(EFillMethod fillMethod) { fFillMethod = fillMethod; }


    /** Empty trees are written by TreeWriters to the root file, if this is set to @e true.
      * Setting this to @e false won't write empty trees to root files - this will avoid lot of empty trees in the root file.
      */
    virtual void SetSaveNonemptyTrees(bool saveNonemptyTrees = true) { fSaveOnlyNonemptyTrees = saveNonemptyTrees; }
    /** The counterpart to SetSaveNonemptyTrees(). */
    virtual bool GetSaveNonemptyTrees() { return fSaveOnlyNonemptyTrees; }

    // Provide a means to clear the fields to be filled to the tree when
    // there are multiple processors writing to the same tree. While this
    // function is not used within the autofilling routines, it can be useful
    // for compound processors
    virtual void Clear() = 0;

  protected:
    // Turn off auto filling: the derived class will explicitly fill the
    // tree. In this case, the tree must have a unique name. Note that this
    // function is only available to derived classes.
    virtual void SetDoNotAutoFillTree();

  protected:
    /*! InitializeTree initializes a tree to use, calling the derived classes
     *  InitializeBranches.  In particular, it adds several 'default' branches,
     *  run number (int), sub-run number (int), and running state (string)
     *  as branches to the tree:
     *
     *  runNumber - Integer, number of the current run
     *  subRunNumber - Integer, number of the current sub run
     *  runningState - string, string representation of the current running
     *  state.  See ORRunContext::EState for a listing of states in the run
     */
    virtual EReturnCode InitializeTree();
    virtual EReturnCode InitializeBranches() = 0;

  protected:
    std::string fTreeName;
    TTree* fTree;
    TBranch* fBranch;
    bool fThisProcessorAutoFillsTree;
    bool fUniqueTree;
    int fFillPeriod;
    EFillMethod fFillMethod;
    int fFillCount;
    EReturnCode fLastProcessedRecordRetCode;
    Bool_t fSaveOnlyNonemptyTrees; //!< flag to skip writing empty trees -tb- 2008-07-25
};

#endif
