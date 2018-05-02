/* Copyright (c) 2018 Big Ladder Software LLC. All rights reserved.
* See the LICENSE file for additional terms and conditions. */

#ifndef GRIDDEDDATA_H_
#define GRIDDEDDATA_H_

#include <vector>
#include <cfloat>

#include "Eigen/Dense"


namespace Btwxt {

    enum method {
        CONSTANT, LINEAR, CUBIC
    };

    class GridAxis {
        // A single input dimension of the performance space
    public:
        GridAxis();

        GridAxis(double *grid, std::size_t size,
                 int extrapolation_method = CONSTANT,
                 int interpolation_method = LINEAR);

        GridAxis(std::vector<double> grid_vector,
                 method extrapolation_method = CONSTANT,
                 method interpolation_method = LINEAR,
                 std::pair<double, double> extrapolation_limits = {-DBL_MAX, DBL_MAX}
        );

        const std::vector<double> grid;
        std::vector<std::vector<double> > spacing_multiplier;
        method extrapolation_method;
        method interpolation_method;
        std::pair<double, double> extrapolation_limits;

        // std::pair<double> extrapolation_bounds;  <-- to add later
        // bool is_regular;  <-- to add later

        std::size_t get_length();

        void set_interp_method(const method interpolation_method);
        void set_extrap_method(const method extrapolation_method);
        void set_extrap_limits(const std::pair<double, double> extrap_limits);

        double get_spacing_multiplier(const std::size_t &flavor, const std::size_t &index);

    private:
        std::vector<std::vector<double> > calc_spacing_multipliers();
        void check_grid_sorted();
        void check_extrap_limits();
    };


    class GriddedData {
    public:
        GriddedData();

        GriddedData(
                std::vector<std::vector<double> > grid,
                std::vector<std::vector<double> > values
        );

        GriddedData(
                std::vector<GridAxis> grid_axes,
                std::vector<std::vector<double> > values
                );

        GriddedData(
                std::vector<GridAxis> grid_axes
        );

        std::size_t get_ndims();

        std::size_t get_num_tables();

        void add_value_table(std::vector<double>& value_vector);

        std::vector<double> get_grid_vector(const std::size_t &dim);

        std::pair<double, double> get_extrap_limits(const std::size_t &dim);

        std::vector<double> get_values(const std::vector<std::size_t> &coords);

        template <typename T>
        Eigen::ArrayXd get_column(const std::vector<T> &coords);

        Eigen::ArrayXd get_column_near(
                std::vector<std::size_t> coords, const std::size_t &dim, const int &i);

        Eigen::ArrayXd get_column_near(
                std::vector<std::size_t> coords, const std::vector<int> &translation);

        Eigen::ArrayXd get_column_near_safe(
                const std::vector<std::size_t>& coords, std::vector<int> translation);

        double get_axis_spacing_mult(const std::size_t &dim,
                                     const std::size_t &flavor, const std::size_t &index);

        std::vector<method> get_interp_methods();

        std::vector<method> get_extrap_methods();
        // double get_value(std::size_t table_index, std::vector<std::size_t> coords);

        void set_axis_extrap_method(const std::size_t &dim, const method);

        void set_axis_extrap_limits(const std::size_t &dim,
                                    const std::pair<double, double> &extrap_limits);

        void set_axis_interp_method(const std::size_t &dim, const method);

        Eigen::ArrayXXd value_tables;
        std::size_t num_values;
        std::vector<std::size_t> dimension_lengths;

    private:
        void construct_axes(const std::vector<std::vector<double> > &grid);

        Eigen::ArrayXXd construct_values(std::vector<double> &value_vector);
        Eigen::ArrayXXd construct_values(
                const std::vector<std::vector<double> > &values
        );
        Eigen::Map<Eigen::ArrayXd> fill_value_row(std::vector<double> &value_vector,
                const std::size_t& num_values);
        std::vector<GridAxis> grid_axes;
        std::size_t ndims;
        std::size_t num_tables;
        // TODO move interp_method down to GridAxis
        std::vector<method> interp_methods;
    };


// free functions
    bool free_check_sorted(std::vector<double>);

    template <typename T>
    std::size_t locate_coords(
            const std::vector<T> &coords,
            const std::vector<std::size_t> &dimension_lengths
    );

    std::vector<double> eigen_to_vector(Eigen::ArrayXd &);


}
#endif // GRIDDEDDATA_H_
