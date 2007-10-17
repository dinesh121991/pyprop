#ifndef DISTRIBUTEDMODEL_H
#define DISTRIBUTEDMODEL_H

#include "../wavefunction.h"
#include "../configuration.h"
#include "../hltimer.h"
#include <iostream>

/* 
 * Class to keep track of the current distribution among processors.
 */
class Distribution
{
public:
	typedef shared_ptr< Distribution > Ptr;
	typedef blitz::Array<int, 1> DataArray;

	Distribution() : Distrib(0) {}

	Distribution(int procRank)
	{
		Distrib.resize(procRank);
	}

	Distribution(const Distribution &other)
	{
		if (other.Distrib.size() > 0)
		{
			this->Distrib.resize(other.Distrib.shape());
			this->Distrib = other.Distrib;
		}
	}

	int GetProcRank() const
	{
		return Distrib.extent(0);
	}
		
	const DataArray GetDistribution() const
	{
		return Distrib;
	}

	void SetDistribution(const DataArray &distrib)
	{
		Distrib = distrib;
	}

private:
	DataArray Distrib;
};
typedef boost::shared_ptr<Distribution> DistributionPtr;

//Forward declaration of arraytranspose
template<int Rank> class ArrayTranspose;

template<int Rank>
class DistributedModel
{
public:
	typedef shared_ptr< DistributedModel<Rank> > Ptr;
	typedef ArrayTranspose<Rank> TransposeType;
	typedef shared_ptr<TransposeType> TransposePtr;
	typedef shared_ptr< DistributedModel<1> > DistributedModel1DPtr;

private:
	Distribution::Ptr CurrentDistribution;
	TransposePtr Transpose;

	static bool MPIDisabled;
	
public:

	int ProcId;
	int ProcCount;
	
	//constructor
	DistributedModel();
	DistributedModel(DistributionPtr distrib);
	DistributedModel(const DistributedModel &other)
	{
		this->CurrentDistribution = DistributionPtr(new Distribution(*other.CurrentDistribution));
		this->Transpose = other.Transpose;
		this->ProcCount = other.ProcCount;
		this->ProcId = other.ProcId;
	}

	/*
	 * Create a 1D distributed model sharing CurrentDistribution with this DistributedModel
	 * This is used by CombinedRepresentation to construct lower dimensional representations which
	 * still knows the correct distribution
	 */
	DistributedModel1DPtr CreateSubDistributedModel()
	{
		return DistributedModel1DPtr( new DistributedModel<1>(CurrentDistribution) );
	}

	bool IsSingleProc()
	{
		return ProcCount == 1;
	}

	bool IsFirstProc()
	{
		return ProcId == 0;
	}

	bool IsLastProc()
	{
		return ProcId == ProcCount -1;
	}
	
	Distribution::DataArray GetDistribution()
	{
		return CurrentDistribution->GetDistribution();
	}

	TransposePtr GetTranspose()
	{
		return Transpose;
	}
	
	template<class T> const blitz::Array<T, 1> GetLocalArray(const blitz::Array<T, 1> &array, int rank)
	{
		return array(GetLocalIndexRange(array.extent(0), rank));
	}

	static void ForceSingleProc()
	{
		MPIDisabled = true;
	}

	void SetupMPI();
	blitz::TinyVector<int, Rank> CreateInitialShape(const blitz::TinyVector<int, Rank> &fullShape);
	int GetLocalStartIndex(int globalSize, int currentRank);
	blitz::Range GetLocalIndexRange(int globalSize, int currentRank);
	void ApplyConfigSection(const ConfigSection &cfg);

	double GetGlobalSum(double localValue);
	cplx GetGlobalSum(cplx localValue);
	void GlobalBarrier();

	bool IsDistributedRank(int rank);
	void ChangeDistribution(Wavefunction<Rank> &psi, const Distribution::DataArray &newDistrib, int destBufferName);

	static void InitMPI(int argc, char* argv[]);
	static void FinalizeMPI();
};

template<int Rank>
bool DistributedModel<Rank>::MPIDisabled = false;

#endif

