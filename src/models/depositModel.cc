#include "depositModel.h"

#include <numeric>
using namespace s21;

/**
 * @brief check is any long year in period days from today
 *
 * @param days period length in days
 * @return int 0 if no long years >0 if it is
 */
int check_long_year(int days) {
  int result = 0;
  time_t current_time, future_time;
  /* Obtain current time. */
  current_time = time(NULL);
  future_time = current_time + days * kSecondsPerDay;
  struct tm current_date = *localtime(&current_time);
  struct tm future_date = *localtime(&future_time);
  for (int i = current_date.tm_year; i <= future_date.tm_year; i++) {
    if (i % 4 == 0) result++;
  }
  return result;
}

/**
 * @brief Get the number of days in the 'period' months from today+startday
 *
 * @param startday number of dayss to add to current date
 * @param period - number of months to add to start date
 * @return int number of days in 'period' moths from start date
 */
int accurate_days_per_period(int startday, int period) {
  int result = 0;
  time_t start_time, future_time;
  /* Obtain current time. */
  start_time = time(NULL);
  struct tm start_date = *localtime(&start_time);
  int mday = start_date.tm_mday;
  if (mday > 28) start_time -= 3 * kSecondsPerDay;
  start_time += startday * kSecondsPerDay;
  start_date = *localtime(&start_time);
  struct tm future_date = start_date;
  future_date.tm_year =
      start_date.tm_year + floor((start_date.tm_mon + period) / 12.);
  future_date.tm_mon = (start_date.tm_mon + period) % 12;

  future_time = mktime(&future_date);
  result = round(difftime(future_time, start_time) / kSecondsPerDay);
  return result;
}

long double bank_round(long double value) {
  int result = (int)value;
  if (value >= 0) {
    if (value < 2147483647.5) {
      long double dif = value - result;
      if (dif > 0.5 || (dif == 0.5 && (result & 1) != 0)) result++;
    }
  }
  return result;
}

/**
 * @brief Get the number of days in the 'period' months from today
 *
 * @param period - number of months to add to current date
 * @return int number of days in 'period' moths from current date
 */
int get_days_per_period(int period) {
  int result = 0;
  result = accurate_days_per_period(0, period);
  return result;
}

/**
 * @brief return number of days per year for date today+days
 *
 * @param days period from today in days
 * @return 366 or 365
 */
int get_days_per_year(int days) {
  int result = 0;
  time_t current_time, future_time;
  /* Obtain current time. */
  current_time = time(NULL);
  future_time = current_time + days * kSecondsPerDay;
  struct tm future_date = *localtime(&future_time);
  if (future_date.tm_year % 4 == 0)
    result = 366;
  else
    result = 365;
  return result;
}

int has_new_year(int start_day, int days) {
  int result = 0;
  time_t start_time, future_time;
  /* Obtain current time. */
  start_time = time(NULL);
  start_time += start_day * kSecondsPerDay;
  future_time = start_time + days * kSecondsPerDay;
  struct tm start_date = *localtime(&start_time);
  struct tm future_date = *localtime(&future_time);
  if (future_date.tm_year != start_date.tm_year) result = 1;
  return result;
}

int get_days_until_new_year(int start_day) {
  int result = 0;
  time_t start_time, future_time;
  /* Obtain current time. */
  start_time = time(NULL);
  start_time += start_day * kSecondsPerDay;
  struct tm start_date = *localtime(&start_time);
  struct tm future_date = start_date;
  future_date.tm_mon = 11;
  future_date.tm_mday = 31;

  future_time = mktime(&future_date);
  result = round(difftime(future_time, start_time) / kSecondsPerDay);
  return result;
}

long double DepositModel::calcSimpleDailyInterest(long double sum, int startday,
                                                  int days) {
  long double res;
  int days_to_new_year = get_days_until_new_year(startday);
  if (days_to_new_year > days) {
    res = round(sum * data_->rate * days / get_days_per_year(startday + days)) /
          100.;
  } else {
    //! recursive calculation gives bad accuracy
    // res = round(sum * data_->rate * days_to_new_year /
    //             get_days_per_year(startday)) /
    //           100. +
    //       calcSimpleDailyInterest(sum, startday + days_to_new_year + 1,
    //                                  days - days_to_new_year);

    res = bank_round(sum * data_->rate * days_to_new_year /
                         get_days_per_year(startday) +
                     sum * data_->rate * (days - days_to_new_year) /
                         get_days_per_year(startday + days)) /
          100.;
  }
  return res;
}

double DepositModel::calcComplexInterest() {
  double res = data_->amount;
  int term = get_days_per_period(data_->duration);
  if (data_->pay_period > term) data_->pay_period = term;
  int period = data_->pay_period;
  int rest = term % period;
  double current_interest;
  data_->interests.clear();
  double replenishment = data_->replenishment;
  // std::accumulate(data_->replenishments.begin(),
  //                                        data_->replenishments.end(), 0);
  double withdrawal = data_->withdrawal;
  // std::accumulate(data_->withdrawals.begin(), data_->withdrawals.end(), 0);
  data_->interest = 0;

  for (int i = 0, n = 0; i < term - rest; i += period, n++) {
    if (data_->pay_period >= 30)
      period = accurate_days_per_period(i, data_->pay_period / 30);

    current_interest = calcSimpleDailyInterest(
        data_->int_cap ? res : data_->amount, i, period);
    data_->interest += current_interest;
    res += current_interest;
    data_->interests.push_back(current_interest);
    if (i + period < term - rest || (data_->pay_period == 7 && rest != 0)) {
      res += replenishment;
      if (res >= withdrawal) res -= withdrawal;
      data_->amount += replenishment;
      if (data_->amount >= withdrawal) data_->amount -= withdrawal;
    }
  }

  if (data_->pay_period == 7) {
    current_interest = calcSimpleDailyInterest(
        data_->int_cap ? res : data_->amount, term - rest - 1, rest);
    data_->interest += current_interest;
    res += current_interest;
    data_->interests.push_back(current_interest);
  }

  return res;
}
