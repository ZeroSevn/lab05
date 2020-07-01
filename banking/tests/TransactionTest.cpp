#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Account.h>
#include <Transaction.h>

using ::testing::AtLeast;
using ::testing::Return;

class MockAccount : public Account {
public:
  MockAccount(int id, int balance) : Account(id, balance) {}

  MOCK_METHOD(void, ChangeBalance, (int), (override));
  MOCK_METHOD(void, Lock, (), (override));
  MOCK_METHOD(void, Unlock, (), (override));
};

TEST(Transaction, default_construcion) { ASSERT_EQ(Transaction().fee(), 1); }

TEST(Transaction, make_same) {
  Transaction transaction;
  MockAccount account1(1, 123), account2(1, 456);

  ASSERT_THROW(transaction.Make(account1, account2, 15), std::logic_error);
}

TEST(Transaction, fee_access) {
  Transaction transaction;

  transaction.set_fee(255);
  ASSERT_EQ(transaction.fee(), 255);
}

TEST(Transaction, fee_const_access) {
  const Transaction transaction;

  ASSERT_EQ(transaction.fee(), 1);
}

TEST(Transaction, make_negative) {
  Transaction transaction;
  MockAccount account1(1, 2), account2(3, 4);

  ASSERT_THROW(transaction.Make(account1, account2, -123),
               std::invalid_argument);
}

TEST(Transaction, make_small_sum) {
  Transaction transaction;
  MockAccount account1(8, 800), account2(555, 3535);

  ASSERT_THROW(transaction.Make(account1, account2, 99), std::logic_error);
}

TEST(Transaction, transaction_too_small_sum) {
  Transaction transaction;
  transaction.set_fee(100);

  MockAccount account1(5, 1000), account2(7, 1000);
  for (int i = 100; i < 2 * 100; ++i)
    ASSERT_FALSE(transaction.Make(account1, account2, i));
}

TEST(Transaction, transaction_too_expensive) {
  Transaction transaction;
  transaction.set_fee(200);

  MockAccount account1(5, 1000), account2(7, 1000);
  ASSERT_FALSE(transaction.Make(account1, account2, 5000));
  ASSERT_FALSE(transaction.Make(account2, account1, 2000));
}
