#pragma once

#include <cmath>
#include <vector>


#ifndef M_PI
#define M_PI ((double)3.141592653589793)
#endif

namespace ipl
{

	template <typename Tv, typename Tt>
	class interpolated_value
	{
	public:
		const Tt kNullTime = -1.f;
		const Tv kNullValue = Tv();



	protected:
		size_t m_maxNum;
		std::vector<Tt> m_time;
		std::vector<Tv> m_value;



	public:
		interpolated_value(size_t num = 20)
			: m_maxNum(num)
			, m_time(num, kNullTime)
			, m_value(num, kNullValue)
		{
		}



	public:
		size_t count() const
		{
			for (size_t i = 0; i < m_time.size(); i++)
				if (m_time[i] == kNullTime)
					return i;

			return m_time.size();
		}

		float lastUpdateTime() const
		{
			return m_time[0];
		}

		const Tv& lastValue() const
		{
			return getValue(0);
		}

		const Tv& lastDelta() const
		{
			return getDelta(0);
		}

		float getUpdateTime(size_t index) const
		{
			return index < m_maxNum ? m_time[index] : kNullTime;
		}

		const Tv& getValue(size_t index) const
		{
			return index < m_maxNum ? m_value[index] : kNullValue;
		}

		virtual Tv getDelta(size_t index) const
		{
			if (index + 1 >= m_maxNum)
				return kNullValue;

			if (m_time[index] < 0 || m_time[index + 1] < 0)
				return kNullValue;

			return m_value[index] - m_value[index + 1]; //TODO: Checked, is it Time?
		}

		void updateTime(const Tt& time, size_t index)
		{
			if (index < m_maxNum) {
				m_time[index] = time;
			}
		}

		void updateValue(const Tv& value, size_t index)
		{
			if (index < m_maxNum) {
				m_value[index] = value;
			}
		}

		void putValue(const Tv& value, Tt time)
		{
			if (std::abs(time - m_time[0]) >= 0.000001) {
				shift();
			}

			m_time[0] = time;
			m_value[0] = value;
		}

		Tv interpolateValue(Tt time) const
		{
			size_t index = findTimeIndex(time);

			if (index == m_maxNum)
				return kNullValue;

			if (index == 0)
				return extrapolateValue(time);

			Tt totalTime = time - m_time[index];
			Tt deltaTime = m_time[index - 1] - m_time[index];
			Tt timeAlpha = totalTime / deltaTime;
			Tv deltaValue = getDelta(index - 1);

			if (std::abs(deltaTime) < 0.000001) {
				timeAlpha = 1.0;
			}

			Tv value = m_value[index] + deltaValue * timeAlpha;
			return value;
		}

		Tv extrapolateValue(Tt time) const
		{
			if (m_time[0] == kNullTime || m_time[1] == kNullTime || time <= m_time[0])
				return kNullValue;

			Tv deltaValue = getDelta(0);

			Tt totalTime = time - m_time[0];
			Tt deltaTime = m_time[0] - m_time[1];
			Tt timeAlpha = totalTime / deltaTime;

			if (std::abs(deltaTime) < 0.000001)	{
				timeAlpha = 1.0;
			}

			Tv value = m_value[0] + deltaValue * timeAlpha;
			return value;
		}

		void clear()
		{
			for (int i = 0; i < m_maxNum; i++) {
				m_time[i] = kNullTime;
				m_value[i] = kNullValue;
			}
		}

	protected:
		void shift()
		{
			for (size_t i = m_maxNum - 1; i > 0; i--) {
				m_time[i] = m_time[i - 1];
				m_value[i] = m_value[i - 1];
			}
		}

		size_t findTimeIndex(Tt time) const
		{
			for (int i = 0; i < m_maxNum; i++) {
				if (m_time[i] == kNullTime)
					break;

				if (m_time[i] < time)
					return i;
			}

			return m_maxNum;
		}
	};

	template <typename Tv, typename Tt>
	class interpolated_angle_value : public interpolated_value<Tv, Tt>
	{
	protected:
		Tv min_value_;
		Tv max_value_;



	public:
		interpolated_angle_value(Tv min_value = -180.0, Tv max_value = 180.0, size_t num = 20)
			: interpolated_value<double,T>(num)
			, min_value_(min_value)
			, max_value_(max_value)
		{}

		virtual double getDelta(size_t index) const
		{
			if (index + 1 >= this->m_maxNum)
				return this->kNullValue;

			if (this->m_time[index] < 0 || this->m_time[index + 1] < 0)
				return this->kNullValue;

			return std::fmod(std::fmod((this->m_value[index] - this->m_value[index + 1]), max_value_ - min_value_) + max_value_ - min_value_ - min_value_, max_value_ - min_value_) + min_value_;
		}
	};
}
