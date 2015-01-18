// http://www.martinbroadhurst.com/cpp-sorting.html
// https://codereview.stackexchange.com/questions/49459/merge-sort-algorithm-implementation-using-c

/*
Merge Sort
*/

template<typename I>
void doMerge(I begin, I midPoint, I end)
{
    typename std::vector<typename std::iterator_traits<I>::value_type> TmpVec;

    TmpVec  tmp(std::make_move_iterator(begin), std::make_move_iterator(end));

    TmpVec::iterator   beginAlt   = std::begin(tmp);
    TmpVec::iterator   endAlt     = std::end(tmp);
    TmpVec::iterator   midAlt     = std::next(beginAlt, std::distance(begin, midPoint));


    TmpVec::iterator   l   = beginAlt
    TmpVec::iterator   r   = midAlt;
    I                  i   = begin;

    while(l < midAlt && r < endAlt)
    {
        *i = std::move((*l < *r) ? *l++ : *r++);
        ++i;
    }
    while(l < midAlt)
    {   *i  = std::move(*l++);
        ++i;
    }
    while(r < endAlt)
    {   *i  = std::move(*r++);
        ++i;
    }
}
template<typename I>
void mergeSort(I begin, I end)
{
    std::size_t length  = std::distance(begin, end);
    if (length <= 1)
    {   return;
    }

    std::size_t mid      = length/2;
    I           midPoint = std::next(begin, mid);

    mergeSort(begin, midPoint);
    mergeSort(midPoint, end);

    doMerge(begin, midPoint, end);
}

int main()
{
    std::vector<int>    data  {{ 5,12,45,2,67,8}};
    mergeSort(std::begin(data), std::end(data));

    std::copy(std::begin(data), std::end(data), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << "\n";
}

/*
Bubble Sort

In a bubble sort, a series of passes is made over the data, and in each pass, every pair of adjacent elements is compared. If they are in the wrong order, they are swapped. The sort completes when no changes have been made in a pass. In each pass the maximum element is "bubbled" to the end, as it will be swapped with every succeeding element, and consequently becoming a part of the next comparison. The effect of this is that a sorted sublist is built up at the right of the input list. The end of the sublist to be sorted is shortened by one element after each pass to avoid examining the sorted sublist.
*/

template <class RandomAccessIterator>
void bubble_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    bool finished = false;
    RandomAccessIterator it, end = last - 1;
    while (!finished) {
        bool changed = false;
        for (it = first; it < end; ++it) {
            if (*it > *(it + 1)) {
                std::swap(*it, *(it + 1));
                changed = true;
            }
        }
        if (changed) {
            --end;
        }
        else {
            finished = true;
        }
    }
}


/*
Selection Sort

In a selection sort the data is searched for the minimum (or maximum) element, and this is swapped with the element in its place at the end. As with a bubble sort, the end of the range to be sorted is adjusted after each pass as a sorted sublist is built up at the end. The construction of the sorted range is identical to that in a bubble sort, but it is performed much more efficiently because there are fewer swaps.
*/

template <class RandomAccessIterator>
void selection_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    RandomAccessIterator begin = first;
    while (begin < last) {
        RandomAccessIterator it, min;
        bool started = false;
        for (it = begin; it < last; ++it) {
            if (!started || *it < *min) {
                min = it;
            }
            started = true;
        }
        std::swap(*min, *begin);
        ++begin; 
    }
}

/*
Min-Max Sort
One can perform a selection sort and simultaneously search for the minimum and maximum element and move them to their proper places in each pass. This has the effect of building up a sorted sublist at each end, with the unsorted part becoming smaller from each ends, as in a cocktail sort. Again, the construction of the sorted ranges is performed much more efficiently in this sort than a cocktail sort because there are fewer swaps.
*/

template <class RandomAccessIterator>
void minmax_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    RandomAccessIterator begin = first;
    RandomAccessIterator end = last - 1;
    while (begin < end) {
        RandomAccessIterator it, min, max;
        bool started = false;
        for (it = begin; it <= end; ++it) {
            if (!started) {
                min = it;
                max = it;
                started = true;
            }
            else {
                if (*it < *min) {
                    min = it;
                }
                else if (*it > *max) {
                    max = it;
                }
            }
        }
        if (max == begin && min == end) {
            // Swap min and max only
            std::swap(*max, *min);
        }
        else if (max == begin) {
            // Swap max and end first
            std::swap(*max, *end);
            std::swap(*min, *begin);
        }
        else {
            // Swap min and begin first
            std::swap(*min, *begin);
            std::swap(*max, *end);
        }
        ++begin; 
        --end;
    }
}