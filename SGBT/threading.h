#ifndef SGBT_UTILS_THREADING_H_
#define SGBT_UTILS_THREADING_H_
#include<omp.h>
#include<functional>
namespace SGBT
{
	class Treading
	{
	public:
		template<Index_T>
		static inline void For(Index_T start, Index_T end, std::function<void(int, Index_T, Index_T)>&inner_func)
		{
			int threadNums = 1;
			#pragma omp parallel
			#pragma omp master
			{
				threadNums = omp_get_num_threads();
			}
			Index_T inner_nums = (end - start + threadNums - 1) / threadNums;
			if (inner_nums <= 0) { inner_nums = 1; }
			#pragma omp parallel for schedule(static,1)
			for (int i = 0; i < threadNums; i++)
			{
				Index_T inner_start = start + i*inner_nums;
				Index_T inner_end = inner_start + inner_nums;
				if (inner_end > end) { inner_end = end; }
				if (inner_start < end)
				{
					inner_func(i, inner_start, inner_end);
				}
			}
		}
	};
}
#endif // !SGBT_UTILS_THREADING_H_
