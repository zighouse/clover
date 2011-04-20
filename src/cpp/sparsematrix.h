/*
 *  Copyright 2011 Zhigang Xie
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  Created on: 2011-4-12
 *      Author: Zhigang Xie
 */

#ifndef SPARSEMATRIX_H_
#define SPARSEMATRIX_H_

#include <map>
#include "clovertypes.h"

namespace clover {

template<typename _MatIter> class MatrixCell;
template<typename _MatIter> class MatrixConstCell;

template<typename _E, typename _RK, typename _CK, typename _RComp, typename _CComp>
class SparseMatrixIterator;

template<typename _E, typename _RK, typename _CK, typename _RComp, typename _CComp>
class SparseMatrixConstIterator;

template<typename _E, typename _RK, typename _CK, typename _RComp, typename _CComp>
class SparseMatrix;

template<typename _MatIter>
class MatrixCell {
public:
	typedef _MatIter iterator;

private:
	iterator * it;
	FRIEND_TYPE(iterator);

public:
	MatrixCell(iterator * _it) : it(_it) {}

	MatrixCell(iterator & _it) : it(&_it) {}

	~MatrixCell() {}

	typename iterator::rowkey_t row() const {
		return it->row->first;
	}

	typename iterator::colkey_t col() const {
		return it->cell->first;
	}

	typename iterator::element_t * elt() const {
		return &it->cell->second;
	}
};

template<typename _E, typename _RK=int, typename _CK=int,
		typename _RComp=std::less<_RK>, typename _CComp=std::less<_CK> >
class SparseMatrixIterator {
public:
	typedef _RK	rowkey_t;
	typedef _CK colkey_t;
	typedef _E	element_t;
	typedef std::map<_CK,_E,_CComp>		row_t;
	typedef std::map<_RK,row_t,_RComp>	mat_t;
	typedef typename row_t::iterator row_iter;
	typedef typename mat_t::iterator mat_iter;
	typedef SparseMatrixIterator<_E,_RK,_CK,_RComp,_CComp> self_t;
	typedef MatrixCell<self_t>	cell_t;

private:
	cell_t m_cell;
	row_iter cell;
	mat_iter row;
	mat_iter mat_end;
	_RK row_lower;
	_RK row_upper;
	_CK col_lower;
	_CK col_upper;
	bool for_all;
	friend class SparseMatrix<_E,_RK,_CK,_RComp,_CComp>;
	friend class MatrixCell<self_t>;
	friend class SparseMatrixConstIterator<_E,_RK,_CK,_RComp,_CComp>;

public:
	SparseMatrixIterator()
	: m_cell(cell_t(this))
	, for_all(true)
	{}

	SparseMatrixIterator(const SparseMatrixIterator & it)
	: m_cell(cell_t(this))
	, for_all(it.for_all)
	{
		cell = it.cell;
		row = it.row;
		mat_end = it.mat_end;
		row_lower = it.row_lower;
		row_upper = it.row_upper;
		col_lower = it.col_lower;
		col_upper = it.col_upper;
	}

	SparseMatrixIterator& operator = (const SparseMatrixIterator & it)
	{
		cell = it.cell;
		row = it.row;
		mat_end = it.mat_end;
		row_lower = it.row_lower;
		row_upper = it.row_upper;
		col_lower = it.col_lower;
		col_upper = it.col_upper;
		for_all = it.for_all;
		return *this;
	}

	SparseMatrixIterator& operator ++ ()
	{
		if(row != mat_end) {
			if(cell != row->second.end()) {
				++ cell;
				while(row != mat_end
						&& (cell == row->second.end() || (!for_all && cell->first >= col_upper) )
						) {
					++ row;
					if(row != mat_end) {
						if(!for_all && row->first >= row_upper) {
							row = mat_end;
						} else {
							if(for_all)
								cell = row->second.begin();
							else
								cell = row->second.lower_bound(col_lower);
						}
					}
				}
			}
		}

		return *this;
	}

	SparseMatrixIterator  operator ++ (int)
	{
		SparseMatrixIterator ret(*this);
		operator ++();
		return ret;
	}

	bool operator == (const SparseMatrixIterator & it) const
	{
		if(it.row == it.mat_end)
			return row == mat_end;
		else
			return cell == it.cell && row == it.row;
	}

	bool operator != (const SparseMatrixIterator & it) const
	{
		return ! operator == (it);
	}

	cell_t * operator -> ()
	{
		if(row == mat_end || cell == row->second.end())
			return 0;
		else
			return &m_cell;
	}

	cell_t & operator * ()
	{
		return m_cell;
	}

	operator cell_t * ()
	{
		if(row == mat_end || cell == row->second.end())
			return 0;
		else
			return &m_cell;
	}
};

template<typename _MatIter>
class MatrixConstCell {
public:
	typedef _MatIter iterator;

private:
	iterator * it;
	FRIEND_TYPE(iterator);

public:
	MatrixConstCell(iterator * _it) : it(_it) {}

	MatrixConstCell(iterator & _it) : it(&_it) {}

	~MatrixConstCell() {}

	typename iterator::rowkey_t row() const {
		return it->row->first;
	}

	typename iterator::colkey_t col() const {
		return it->cell->first;
	}

	typename iterator::element_t const * elt() const {
		return &it->cell->second;
	}
};

template<typename _E, typename _RK=int, typename _CK=int,
		typename _RComp=std::less<_RK>, typename _CComp=std::less<_CK> >
class SparseMatrixConstIterator {
public:
	typedef _RK	rowkey_t;
	typedef _CK colkey_t;
	typedef _E	element_t;
	typedef std::map<_CK,_E,_CComp>		row_t;
	typedef std::map<_RK,row_t,_RComp>	mat_t;
	typedef typename row_t::const_iterator row_iter;
	typedef typename mat_t::const_iterator mat_iter;
	typedef SparseMatrixIterator<_E,_RK,_CK,_RComp,_CComp> iterator_t;
	typedef SparseMatrixConstIterator<_E,_RK,_CK,_RComp,_CComp> self_t;
	typedef MatrixConstCell<self_t>	cell_t;

private:
	cell_t m_cell;
	row_iter cell;
	mat_iter row;
	mat_iter mat_end;
	_RK row_lower;
	_RK row_upper;
	_CK col_lower;
	_CK col_upper;
	bool for_all;
	friend class SparseMatrix<_E,_RK,_CK,_RComp,_CComp>;
	friend class MatrixConstCell<self_t>;

public:
	SparseMatrixConstIterator()
	: m_cell(cell_t(this))
	, for_all(true)
	{}

	SparseMatrixConstIterator(const SparseMatrixConstIterator & it)
	: m_cell(cell_t(this))
	, for_all(it.for_all)
	{
		cell = it.cell;
		row = it.row;
		mat_end = it.mat_end;
		row_lower = it.row_lower;
		row_upper = it.row_upper;
		col_lower = it.col_lower;
		col_upper = it.col_upper;
	}

	SparseMatrixConstIterator(const iterator_t & it)
	: m_cell(cell_t(this))
	, for_all(it.for_all)
	{
		cell = it.cell;
		row = it.row;
		mat_end = it.mat_end;
		row_lower = it.row_lower;
		row_upper = it.row_upper;
		col_lower = it.col_lower;
		col_upper = it.col_upper;
	}

	SparseMatrixConstIterator& operator = (const SparseMatrixConstIterator & it)
	{
		cell = it.cell;
		row = it.row;
		mat_end = it.mat_end;
		row_lower = it.row_lower;
		row_upper = it.row_upper;
		col_lower = it.col_lower;
		col_upper = it.col_upper;
		for_all = it.for_all;
		return *this;
	}

	SparseMatrixConstIterator& operator = (const iterator_t & it)
	{
		cell = it.cell;
		row = it.row;
		mat_end = it.mat_end;
		row_lower = it.row_lower;
		row_upper = it.row_upper;
		col_lower = it.col_lower;
		col_upper = it.col_upper;
		for_all = it.for_all;
		return *this;
	}

	SparseMatrixConstIterator & operator ++ ()
	{
		if(row != mat_end) {
			if(cell != row->second.end()) {
				++ cell;
				while(row != mat_end
						&& (cell == row->second.end()
								|| (!for_all && cell->first >= col_upper) )
						) {
					++ row;
					if(row != mat_end) {
						if(!for_all && row->first >= row_upper) {
							row = mat_end;
						} else {
							if(for_all)
								cell = row->second.begin();
							else
								cell = row->second.lower_bound(col_lower);
						}
					}
				}
			}
		}

		return *this;
	}

	SparseMatrixConstIterator  operator ++ (int)
	{
		SparseMatrixConstIterator ret(*this);
		operator ++();
		return ret;
	}

	bool operator == (const SparseMatrixConstIterator & it) const
	{
		if(it.row == it.mat_end)
			return row == mat_end;
		else
			return cell == it.cell && row == it.row;
	}

	bool operator == (const iterator_t & it) const
	{
		if(it.row == it.mat_end)
			return row == mat_end;
		else
			return cell == it.cell && row == it.row;
	}

	bool operator != (const SparseMatrixConstIterator & it) const
	{
		return ! operator == (it);
	}

	bool operator != (const iterator_t & it) const
	{
		return ! operator == (it);
	}

	cell_t * operator -> ()
	{
		if(row == mat_end || cell == row->second.end())
			return 0;
		else
			return &m_cell;
	}

	cell_t & operator * ()
	{
		return m_cell;
	}

	operator cell_t * ()
	{
		if(row == mat_end || cell == row->second.end())
			return 0;
		else
			return &m_cell;
	}
};

// SparseMatrix
// Template parameters
// _E - type of element contained by this sparse matrix
// _RK - type of row key
// _CK - type of column key
// _RComp - type of comparer of row key
// _CComp - type of comparer of column key
template<typename _E, typename _RK=int, typename _CK=int,
		typename _RComp=std::less<_RK>, typename _CComp=std::less<_CK> >
class SparseMatrix {
public:
	typedef _RK	rowkey_t;
	typedef _CK colkey_t;
	typedef _E	element_t;
	typedef std::map<_CK,_E,_CComp>		row_t;
	typedef std::map<_RK,row_t,_RComp>	mat_t;
	typedef typename row_t::iterator row_iter;
	typedef typename mat_t::iterator mat_iter;
	typedef typename row_t::const_iterator row_const_iter;
	typedef typename mat_t::const_iterator mat_const_iter;
	typedef SparseMatrixIterator<_E,_RK,_CK,_RComp,_CComp> iterator;
	typedef SparseMatrixConstIterator<_E,_RK,_CK,_RComp,_CComp> const_iterator;

private:
	mat_t mat;

public:
	SparseMatrix(){}

	SparseMatrix(const SparseMatrix & sm)
	{ mat = sm.mat; }

	SparseMatrix & operator =(const SparseMatrix & sm)
	{ mat = sm.mat; return *this; }

	bool empty() const {
		return mat.empty();
	}

	size_t size() const {
		size_t s = 0;
		for(mat_iter it=mat.begin(); it!=mat.end(); ++it) {
			s += it->second.size();
		}
		return s;
	}

	rowkey_t row_lower() const {
		typename mat_t::const_iterator it = mat.begin();
		if(it != mat.end())
			return it->first;
		return 0;
	}

	rowkey_t row_upper() const {
		typename mat_t::const_reverse_iterator it = mat.rbegin();
		if(it != mat.rend())
			return it->first + 1;
		return 0;
	}

	colkey_t col_lower() const {
		colkey_t k=0;
		bool init = false;
		for(mat_const_iter it=mat.begin(); it!=mat.end(); ++it) {
			row_const_iter it2=it->second.begin();
			if(it2!=it->second.end()) {
				if(!init) {
					init = true;
					k = it2->first;
				} else if(k > it2->first) {
					k = it2->first;
				}
			}
		}
		return k;
	}

	colkey_t col_upper() const {
		colkey_t k=0;
		bool init = false;
		for(mat_const_iter it=mat.begin(); it!=mat.end(); ++it) {
			typename row_t::const_reverse_iterator it2=it->second.rbegin();
			if(it2!=it->second.rend()) {
				if(!init) {
					init = true;
					k = it2->first;
				} else if(k < it2->first) {
					k = it2->first;
				}
			}
		}
		if(init)
			return k + 1;
		else
			return 0;
	}

	size_t count(const rowkey_t & row, const colkey_t & col) const {
		mat_iter it = mat.find(row);
		if(it == mat.end()) {
			return 0;
		}
		return it->second.count(col);
	}

	element_t * get(const rowkey_t & row, const colkey_t & col) {
		mat_iter it = mat.find(row);
		if(it == mat.end()) {
			return 0;
		} else {
			row_iter itCell = it->second.find(col);
			if(itCell == it->second.end()) {
				return 0;
			} else {
				return &itCell->second;
			}
		}
		return 0;
	}

	const element_t * get(const rowkey_t & row, const colkey_t & col) const {
		mat_const_iter it = mat.find(row);
		if(it == mat.end()) {
			return 0;
		} else {
			row_const_iter itCell = it->second.find(col);
			if(itCell == it->second.end()) {
				return 0;
			} else {
				return &itCell->second;
			}
		}
		return 0;
	}

	void put(const rowkey_t & row, const colkey_t & col, const element_t & elt) {
		mat_iter it = mat.find(row);
		if(it == mat.end()) {
			std::pair<mat_iter,bool>
				ret = mat.insert(std::pair<rowkey_t,row_t>(row,row_t()));
			it = ret.first;
		}
		it->second.insert(std::pair<colkey_t,element_t>(col, elt));
	}

	size_t erase(const rowkey_t & row, const colkey_t & col) {
		mat_iter itMat = mat.find(row);
		if(itMat != mat.end()) {
			row_iter itRow = itMat->second.find(col);
			if(itRow != itMat->second.end()) {
				itMat->second.erase(itRow);
				if(itMat->second.empty()) {
					mat.erase(itMat);
				}
				return 1;
			}
		}
		return 0;
	}

	void erase(iterator position) {
		if(position.row != mat.end()) {
			position.row->second.erase(position.cell);
			if(position.row->second.empty()) {
				mat.erase(position.row);
			}
		}
	}

	void erase(iterator __first, iterator __last) {
		while(__first != __last) {
			mat_iter itMat = __first.row;
			row_iter itRow = __first.cell;
			++ __first;
			if(itMat != mat.end()) {
				itMat->second.erase(itRow);
				if(itMat->second.empty()) {
					mat.erase(itMat);
				}
			}
		}
	}

	void clear() {
		mat.clear();
	}

	iterator begin(const rowkey_t & row0, const rowkey_t & row1,
			const colkey_t & col0, const colkey_t & col1)
	{
		iterator it;
		it.for_all = false;
		it.row_lower = row0;
		it.row_upper = row1;
		it.col_lower = col0;
		it.col_upper = col1;
		it.mat_end = mat.end();
		it.row = mat.lower_bound(row0);
		if(it.row != mat.end()) {
			if(it.row->first < row1) {
				it.cell = it.row->second.lower_bound(col0);
				while(it.row != mat.end() && it.row->first < row1
						&& (it.cell == it.row->second.end() || it.cell->first >= col1)
						) {
					++ it.row;
					if(it.row != mat.end()) {
						if(it.row->first < row1) {
							it.cell = it.row->second.lower_bound(col0);
						} else {
							it.row = mat.end();
						}
					}
				}
			} else {
				it.row = mat.end();
			}
		}
		return it;
	}

	const_iterator begin(const rowkey_t & row0, const rowkey_t & row1,
			const colkey_t & col0, const colkey_t & col1) const
	{
		const_iterator it;
		it.for_all = false;
		it.row_lower = row0;
		it.row_upper = row1;
		it.col_lower = col0;
		it.col_upper = col1;
		it.mat_end = mat.end();
		it.row = mat.lower_bound(row0);
		if(it.row != mat.end()) {
			if(it.row->first < row1) {
				it.cell = it.row->second.lower_bound(col0);
				while(it.row != mat.end() && it.row->first < row1
						&& (it.cell == it.row->second.end() || it.cell->first >= col1)
						) {
					++ it.row;
					if(it.row != mat.end()) {
						if(it.row->first < row1) {
							it.cell = it.row->second.lower_bound(col0);
						} else {
							it.row = mat.end();
						}
					}
				}
			} else {
				it.row = mat.end();
			}
		}
		return it;
	}

	iterator begin()
	{
		iterator it;
		it.mat_end = mat.end();
		it.row = mat.begin();
		if(it.row != mat.end()) {
			it.cell = it.row->second.begin();
			while(it.row != mat.end() && it.cell == it.row->second.end()) {
				++ it.row;
				if(it.row != mat.end()) {
					it.cell = it.row->second.begin();
				}
			}
		}
		return it;
	}

	const_iterator begin() const
	{
		const_iterator it;
		it.mat_end = mat.end();
		it.row = mat.begin();
		if(it.row != mat.end()) {
			it.cell = it.row->second.begin();
			while(it.row != mat.end() && it.cell == it.row->second.end()) {
				++ it.row;
				if(it.row != mat.end()) {
					it.cell = it.row->second.begin();
				}
			}
		}
		return it;
	}

	iterator end()
	{
		iterator it;
		it.row = mat.end();
		it.mat_end = mat.end();
		return it;
	}

	const_iterator end() const
	{
		const_iterator it;
		it.row = mat.end();
		it.mat_end = mat.end();
		return it;
	}
};

} // namespace clover

#endif /* SPARSEMATRIX_H_ */

