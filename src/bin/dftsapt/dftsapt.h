#ifndef DFTSAPT_H
#define DFTSAPT_H

#include <libmints/typedefs.h>
#include <libmints/wavefunction.h>
#include <map>

namespace psi {

class JK;
class Options;

namespace dftsapt {

class DFTSAPT {

protected:

    // Print flag
    int print_;
    // Debug flag
    int debug_;
    // Bench flag
    int bench_;
    
    // CPKS maximum iterations
    int cpks_maxiter_;
    // CPKS convergence threshold
    double cpks_delta_;

    // Memory in doubles
    unsigned long int memory_;

    // Energies table
    std::map<std::string, double> energies_;

    // Dimer primary basis set
    boost::shared_ptr<BasisSet> primary_;
    // Monomer A primary basis set 
    boost::shared_ptr<BasisSet> primary_A_;
    // Monomer B primary basis set
    boost::shared_ptr<BasisSet> primary_B_;
    // Dimer -RI or -MP2FIT auxiliary basis set 
    boost::shared_ptr<BasisSet> mp2fit_;

    // Dimer SCF energy
    double E_dimer_;
    // Monomer A SCF energy
    double E_monomer_A_;
    // Monomer B SCF energy
    double E_monomer_B_;
    
    // Dimer geometry
    boost::shared_ptr<Molecule> dimer_;
    // Monomer A geometry
    boost::shared_ptr<Molecule> monomer_A_;
    // Monomer B geometry
    boost::shared_ptr<Molecule> monomer_B_;

    // Monomer A C matrix (full occ)
    boost::shared_ptr<Matrix> Cocc_A_;
    // Monomer B C matrix (full occ)
    boost::shared_ptr<Matrix> Cocc_B_;
    // Monomer A C matrix (full vir)
    boost::shared_ptr<Matrix> Cvir_A_;
    // Monomer B C matrix (full vir)
    boost::shared_ptr<Matrix> Cvir_B_;
    
    // Monomer A eps vector (full occ)
    boost::shared_ptr<Vector> eps_occ_A_;
    // Monomer B eps vector (full occ)
    boost::shared_ptr<Vector> eps_occ_B_;
    // Monomer A eps vector (full vir)
    boost::shared_ptr<Vector> eps_vir_A_;
    // Monomer B eps vector (full vir)
    boost::shared_ptr<Vector> eps_vir_B_;


    // Monomer A C matrix (active occ)
    boost::shared_ptr<Matrix> Caocc_A_;
    // Monomer B C matrix (active occ)
    boost::shared_ptr<Matrix> Caocc_B_;
    // Monomer A C matrix (active vir)
    boost::shared_ptr<Matrix> Cavir_A_;
    // Monomer B C matrix (active vir)
    boost::shared_ptr<Matrix> Cavir_B_;

    // Monomer A eps vector (active occ)
    boost::shared_ptr<Vector> eps_aocc_A_;
    // Monomer B eps vector (active occ)
    boost::shared_ptr<Vector> eps_aocc_B_;
    // Monomer A eps vector (active vir)
    boost::shared_ptr<Vector> eps_avir_A_;
    // Monomer B eps vector (active vir)
    boost::shared_ptr<Vector> eps_avir_B_;

    // Monomer A eps vector (frozen occ)
    boost::shared_ptr<Vector> eps_focc_A_;
    // Monomer B eps vector (frozen occ)
    boost::shared_ptr<Vector> eps_focc_B_;
    // Monomer A eps vector (frozen vir)
    boost::shared_ptr<Vector> eps_fvir_A_;
    // Monomer B eps vector (frozen vir)
    boost::shared_ptr<Vector> eps_fvir_B_;
    
    // Print author/sizing/spec info
    virtual void print_header() const;
    // Obligatory
    virtual void print_trailer() const;

    // Hartree-Fock-like terms (Elst, Exch, Ind)
    virtual void fock_terms();
    // MP2-like terms (Disp)
    virtual void mp2_terms();

    // Build the AO-basis dimer overlap matrix
    boost::shared_ptr<Matrix> build_S(boost::shared_ptr<BasisSet> basis);
    // Build the potential integral matrix
    boost::shared_ptr<Matrix> build_V(boost::shared_ptr<BasisSet> basis);

    // Build the S_ij matrix in the dimer occupied space
    boost::shared_ptr<Matrix> build_Sij(boost::shared_ptr<Matrix> S);
    // Build the S^2 expansion in the dimer occupied space
    boost::shared_ptr<Matrix> build_Sij_2(boost::shared_ptr<Matrix> Sij);
    // Build the S^\infty expansion in the dimer occupied space
    boost::shared_ptr<Matrix> build_Sij_n(boost::shared_ptr<Matrix> Sij);
    // Build the Cbar matrices
    std::map<std::string, boost::shared_ptr<Matrix> > build_Cbar(boost::shared_ptr<Matrix> S);

    // Build a generalized density matrix
    boost::shared_ptr<Matrix> build_D(boost::shared_ptr<Matrix> L, boost::shared_ptr<Matrix> R);
    // Build the CPKS RHS (ov-space)
    boost::shared_ptr<Matrix> build_w(boost::shared_ptr<Matrix> W, boost::shared_ptr<Matrix> L, boost::shared_ptr<Matrix> R);

    // Compute the CPKS solution
    std::pair<boost::shared_ptr<Matrix>, boost::shared_ptr<Matrix> > compute_x(boost::shared_ptr<JK> jk, boost::shared_ptr<Matrix> w_B, boost::shared_ptr<Matrix> w_A);
    // Triple GEMM (all matrices must be square)
    boost::shared_ptr<Matrix> triple(boost::shared_ptr<Matrix> A, boost::shared_ptr<Matrix> B, boost::shared_ptr<Matrix> C);
    // Build the C_O matrix
    boost::shared_ptr<Matrix> build_C_O(boost::shared_ptr<Matrix> C, boost::shared_ptr<Matrix> S, boost::shared_ptr<Matrix> P);
    // Build the C_X matrix
    boost::shared_ptr<Matrix> build_C_X(boost::shared_ptr<Matrix> x, boost::shared_ptr<Matrix> C);

    // Protected constructor (use factory below)
    DFTSAPT();
    // Common initialization
    void common_init();
public:
    // Destructor, frees memory
    virtual ~DFTSAPT();

    // Factory constructor, call this with 3 converged SCF jobs (dimer, monomer A, monomer B)
    static boost::shared_ptr<DFTSAPT> build(boost::shared_ptr<Wavefunction> d,
                                            boost::shared_ptr<Wavefunction> mB,
                                            boost::shared_ptr<Wavefunction> mA);

    // Compute the DFT-SAPT analysis
    virtual double compute_energy();

};

class CPKS_SAPT {

friend class DFTSAPT;

protected:

    // => Global Data <= //

    // Convergence tolerance
    double delta_;
    // Maximum allowed iterations
    int maxiter_;
    // JK Object 
    boost::shared_ptr<JK> jk_;
    
    // => Monomer A Problem <= //

    // Perturbation applied to A
    boost::shared_ptr<Matrix> w_A_;
    // Response of A
    boost::shared_ptr<Matrix> x_A_;
    // Active occ orbital coefficients of A
    boost::shared_ptr<Matrix> Cocc_A_;
    // Active vir orbital coefficients of A
    boost::shared_ptr<Matrix> Cvir_A_;
    // Active occ orbital eigenvalues of A
    boost::shared_ptr<Vector> eps_occ_A_;
    // Active vir orbital eigenvalues of A
    boost::shared_ptr<Vector> eps_vir_A_;

    // => Monomer B Problem <= //

    // Perturbation applied to B
    boost::shared_ptr<Matrix> w_B_;
    // Response of B
    boost::shared_ptr<Matrix> x_B_;
    // Active occ orbital coefficients of B
    boost::shared_ptr<Matrix> Cocc_B_;
    // Active vir orbital coefficients of B
    boost::shared_ptr<Matrix> Cvir_B_;
    // Active occ orbital eigenvalues of B
    boost::shared_ptr<Vector> eps_occ_B_;
    // Active vir orbital eigenvalues of B
    boost::shared_ptr<Vector> eps_vir_B_;

    // Form the s = Ab product for the provided vectors b (may or may not need more iterations)
    std::map<std::string, boost::shared_ptr<Matrix> > product(std::map<std::string, boost::shared_ptr<Matrix> > b);
    // Apply the denominator from r into z
    void preconditioner(boost::shared_ptr<Matrix> r,
                        boost::shared_ptr<Matrix> z,
                        boost::shared_ptr<Vector> o,
                        boost::shared_ptr<Vector> v);

public:
    CPKS_SAPT();
    virtual ~CPKS_SAPT();

    void compute_cpks();
};

}} // End namespace

#endif
