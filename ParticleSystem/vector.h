#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <array>

namespace ps
{
	template<class TYPE>
	class vector3
	{
	public:
		vector3();
		vector3(TYPE x, TYPE y, TYPE z);
		vector3(const vector3<TYPE>& other);

		// Random Access Operators
		TYPE& operator[] (unsigned int n)
		{
			return m_data[n];
		}

		const TYPE& operator[] (unsigned int n) const
		{
			return m_data[n];
		}

		// Arithmetic Operators for other vectors
		vector3<TYPE> operator+(const vector3<TYPE>& rhs)
		{
			vector3<TYPE> ret(*this);
			ret += rhs;
			return ret;
		}

		vector3<TYPE>& operator+=(const vector3<TYPE>& rhs)
		{
			for (unsigned int i = 0; i < 3; ++i)
			{
				m_data[i] += rhs[i];
			}
			return *this;
		}

		vector3<TYPE> operator-(const vector3<TYPE>& rhs)
		{
			vector3<TYPE> ret(*this);
			ret -= rhs;
			return ret;
		}

		vector3<TYPE>& operator-=(const vector3<TYPE>& rhs)
		{
			for (unsigned int i = 0; i < 3; ++i)
			{
				m_data[i] -= rhs[i];
			}
			return *this;
		}

		friend vector3<TYPE> operator*(vector3 lhs, const TYPE& rhs)
		{
			return lhs *= rhs;
		}

		friend vector3<TYPE> operator*(const TYPE& lhs, vector3 rhs)
		{
			return rhs *= lhs;
		}

		vector3<TYPE>& operator*=(const TYPE& rhs)
		{
			for (unsigned int i = 0; i < 3; ++i)
			{
				m_data[i] *= rhs;
			}
			return *this;
		}

		friend vector3<TYPE> operator/(vector3 lhs, const TYPE& rhs)
		{
			return lhs /= rhs;
		}

		vector3<TYPE>& operator/=(const TYPE& rhs)
		{
			for (unsigned int i = 0; i < 3; ++i)
			{
				m_data[i] /= rhs;
			}
			return *this;
		}

		vector3<TYPE> operator/(const vector3<TYPE>& rhs)
		{
			vector3<TYPE> ret(*this);
			ret /= rhs;
			return ret;
		}
		
		friend std::ostream& operator<< (std::ostream& stream, const vector3<TYPE>& rhs)
		{
			stream << "[" << rhs[0] << ", " << rhs[1] << ", " << rhs[2] << "]";
			return stream;
		}

		// Mathematical vector operations
		TYPE norm_squared()
		{
			// The euclidean norm squared - the sum of the squares of all the vector elements
			// This is basically just the inner product with itself
			return inner_product(*this, *this);
		}

		TYPE norm()
		{
			// Also known as length or magnitude
			// The euclidean norm - defined as the square root of sum of the squares of all the vector elements
			// Physically this is the length of the vector
			return std::sqrt(norm_squared());
		}

		void normalise()
		{
			auto magnitude = norm();
			for (auto i = 0; i<3; ++i)
			{
				m_data[i] /= magnitude;
			}
		}

		// Dot/Scalar Product a . b, this is the Inner Product as defined for Rn, 
		// which is a way to multiply vectors together that results in a scalar
		friend TYPE inner_product(const vector3<TYPE>& lhs, const vector3<TYPE>& rhs)
		{
			TYPE result = static_cast<TYPE>(0);
			for (int i = 0; i < 3; ++i)
			{
				result += lhs[i] * rhs[i];
			}
			return result;
		}

	private:
		std::array<TYPE, 3> m_data;
	};

	template <class TYPE>
	vector3<TYPE>::vector3() : vector3(0, 0, 0)
	{
	}

	template <class TYPE>
	vector3<TYPE>::vector3(TYPE x, TYPE y, TYPE z)
	{
		m_data[0] = x;
		m_data[1] = y;
		m_data[2] = z;
	}

	template <class TYPE>
	vector3<TYPE>::vector3(const vector3<TYPE>& other)
	{
		m_data[0] = other[0];
		m_data[1] = other[1];
		m_data[2] = other[2];
	}

	using vector3d = vector3<double>;
	using vector3f = vector3<float>;
	using vector3i = vector3<int>;
}

#endif // __VECTOR_H__
