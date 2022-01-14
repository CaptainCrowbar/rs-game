#pragma once

#include "rs-graphics-core/vector.hpp"
#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace RS::Game {

    // Ring number = 0+
    // Step number = 0 if R=0, otherwise 0 to 6R-1, clockwise

    //             ____             //
    //        ____/2,0 \____        //
    //   ____/2,11\____/2,1 \____   //
    //  /2,10\____/1,0 \____/2,2 \  //
    //  \____/1,5 \____/1,1 \____/  //
    //  /2,9 \____/0,0 \____/2,3 \  //
    //  \____/1,4 \____/1,2 \____/  //
    //  /2,8 \____/1,3 \____/2,4 \  //
    //  \____/2,7 \____/2,5 \____/  //
    //       \____/2,6 \____/       //
    //            \____/            //

    namespace Detail {

        Graphics::Core::Int2 rs_to_xy(Graphics::Core::Int2 hex) noexcept;
        Graphics::Core::Int2 xy_to_rs(Graphics::Core::Int2 xy) noexcept;

    }

    class Hexmap {

    public:

        enum class side: int {
            north,
            northeast,
            southeast,
            south,
            southwest,
            northwest,
        };

        enum class xt: int {
            implicit = -1,
            no,
            yes,
        };

        using hex = Graphics::Core::Int2;
        using hexlist = std::vector<hex>;
        using xcolour = Graphics::Core::Int3; // [0-5]^3

        static constexpr xcolour black = {0,0,0};
        static constexpr xcolour white = {5,5,5};

        hexlist all() const;                                      // All set hexes (hexes with at least one attribute), in lexical order
        hexlist border_in() const;                                // All set hexes touching the inside of the border, in lexical order
        hexlist border_out() const;                               // All unset hexes touching the outside of the border, in lexical order
        bool contains(hex h) const noexcept;                      // True if the hex is set
        bool empty() const noexcept { return table_.empty(); }    // True if there are no set hexes
        hexlist neighbours_set(hex h) const;                      // List all set neighbours, in clockwise order
        hexlist neighbours_unset(hex h) const;                    // List all unset neighbours, in clockwise order
        hexlist reachable(hex h) const;                           // List all reachable set hexes (including h if set), in lexical order
        int rings() const;                                        // Ring number of outermost set hex plus one
        int size() const noexcept { return int(table_.size()); }  // Number of set hexes

        void clear() noexcept { table_.clear(); }                 // Delete all attributes, leaving all hexes unset
        void erase(hex h) noexcept { table_.erase(h); }           // Unset a hex

        xcolour colour(hex h) const noexcept;                     // Colour of the hex (black if unset)
        void clear_colour(hex h) noexcept;                        // Colour of the hex
        bool set_colour(hex h, xcolour c) noexcept;               // Colour of the hex
        std::string label(hex h) const;                           // Label of the hex
        void set_label(hex h, const std::string& label);          // Label of the hex
        void set(hex h, const std::string& label);                // Set a hex's label (clear colour)
        void set(hex h, const std::string& label, xcolour c);     // Set a hex's label and colour

        int radius() const noexcept { return radius_; }           // Minimum map radius
        void set_radius(int r = 1) noexcept { radius_ = r; }      // Minimum map radius
        int width() const noexcept { return width_; }             // Minimum hex width
        void set_width(int w = 2) noexcept { width_ = w; }        // Minimum hex width
        xt xterm() const noexcept { return xterm_; }              // Use Xterm colours
        void set_xterm(xt x) noexcept { xterm_ = x; }             // Use Xterm colours
        std::string str() const { return as_string(false); }      // Format as ASCII hex map

        static int area(int radius) noexcept;                     // Number of hexes out to this radius (radius includes centre)
        static int distance(hex h1, hex h2) noexcept;             // Distance (number of steps) between hexes
        static bool is_adjacent(hex h1, hex h2) noexcept;         // True if hexes are adjacent or the same
        static bool is_valid(hex h) noexcept;                     // True if the coordinates are valid
        static hex neighbour(hex h, side s) noexcept;             // Neighbouring hex in the given direction (modulo 6)
        static hexlist neighbours(hex h);                         // List all neighbours, in clockwise order
        static int perimeter(int radius) noexcept;                // Number of hexes on circumference with this radius (includes centre)
        static hex principal(hex h) noexcept;                     // Reduce coordinates to principal values
        static int ring_size(int r) noexcept;                     // Number of hexes on this ring number

        friend std::ostream& operator<<(std::ostream& out, const Hexmap& hm) { return out << hm.as_string(true); }

    private:

        struct hex_info {
            std::string label;
            std::optional<xcolour> colour;
        };

        using hex_table = std::map<hex, hex_info>;
        using xy_pos = Graphics::Core::Int2; // XY coordinates are 2 units per hex in both directions

        hex_table table_;
        int radius_ = 1;
        int width_ = 2;
        xt xterm_ = xt::implicit;

        std::string as_string(bool io) const;

        static xy_pos rs_to_rc(hex h, int n_rings, int width) noexcept;

    };

}
