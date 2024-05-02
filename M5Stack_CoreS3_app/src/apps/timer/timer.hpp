#pragma once

#include <lvgl.h>
#include "../utility.hpp"

namespace app
{

	class timer
	{

		lv_obj_t *parent_;
		lv_style_t style_select_;
		lv_style_t style_cover_;
		lv_obj_ptr_t obj_cover_;
		// lv_obj_ptr_t obj_hr_;
		lv_obj_ptr_t obj_min_;
		lv_obj_ptr_t obj_sec_;
		lv_obj_ptr_t obj_msec_;
		static constexpr size_t obj_width_ = 80;
		static constexpr size_t obj_height_ = 120;
		static constexpr size_t obj_height_msec_ = 80;
		static constexpr size_t obj_margin_ = 10;
		size_t count_min_;
		size_t count_sec_;
		size_t count_msec_;
		//
		bool is_count_;

		// roller option
		// min, sec
		char const *opt_60_ = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n"
							  "10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n"
							  "20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n"
							  "30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n"
							  "40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n"
							  "50\n51\n52\n53\n54\n55\n56\n57\n58\n59";
		// msec
		char const *opt_msec_ = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9";

	public:
		timer();
		bool init(lv_obj_t *parent);

		void begin();

		void count();

	private:
		void reset_timer();

		static void event_cb(lv_event_t *);
	};

}
