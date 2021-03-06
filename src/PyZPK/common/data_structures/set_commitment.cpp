#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/operators.h>
#include <libff/algebra/curves/mnt/mnt6/mnt6_pp.hpp>
#include <libsnark/common/data_structures/set_commitment.hpp>
#include <libsnark/gadgetlib1/gadgets/hashes/knapsack/knapsack_gadget.hpp>

namespace py = pybind11;
using namespace libsnark;
using namespace libff;

void declare_set_membership_proof(py::module &m)
{
    // Binding for common/data_structures/set_commitment.cpp // set_membership_proof
    py::class_<set_membership_proof>(m, "set_membership_proof")
        .def("size_in_bits", &set_membership_proof::size_in_bits)
        .def(
            "__eq__", [](set_membership_proof const &self, set_membership_proof const &other) { return self == other; }, py::is_operator())
        .def("__ostr__", [](set_membership_proof const &proof) {
            std::ostringstream out;
            out << proof.address << "\n";
            out << proof.merkle_path.size() << "\n";
            for (size_t i = 0; i < proof.merkle_path.size(); ++i)
            {
                libff::output_bool_vector(out, proof.merkle_path[i]);
            }
            return out;
        })
        .def("__istr__", [](set_membership_proof &proof) {
            std::istringstream in;
            in >> proof.address;
            libff::consume_newline(in);
            size_t tree_depth;
            in >> tree_depth;
            libff::consume_newline(in);
            proof.merkle_path.resize(tree_depth);
            for (size_t i = 0; i < tree_depth; ++i)
            {
                libff::input_bool_vector(in, proof.merkle_path[i]);
            }
            return in;
        });
}

void declare_set_commitment_accumulator(py::module &m)
{
    using FieldT = Fp_model<5l, libff::mnt46_modulus_B>;
    using HashT = knapsack_CRH_with_field_out_gadget<FieldT>;

    py::class_<set_commitment_accumulator<HashT>>(m, "set_commitment_accumulator")
        .def_readwrite("depth", &set_commitment_accumulator<HashT>::depth)
        .def_readwrite("digest_size", &set_commitment_accumulator<HashT>::digest_size)
        .def_readwrite("value_size", &set_commitment_accumulator<HashT>::value_size);
}

void init_data_structures_set_commitment(py::module &m)
{
    declare_set_membership_proof(m);
    declare_set_commitment_accumulator(m);
}