#include <gtest/gtest.h>

#include <Account.h>


TEST(Account, construction_is_safe) { ASSERT_NO_THROW(Account(123, 456)); }

TEST(Account, id_access) {
ASSERT_EQ(Account(666, 222).id(), 666);
}

TEST(Account, balance_access) {
ASSERT_EQ(Account(666, 222).GetBalance(), 222);
}

TEST(Account, id_const_access) {
Account const account(137, 700);
ASSERT_EQ(account.id(), 137);
}

TEST(Account, balance_const_access) {
Account const account(137, 700);
ASSERT_EQ(account.GetBalance(), 700);
}

TEST(Account, id_access_under_lock) {
Account account(400, 142);

account.Lock();
ASSERT_NO_THROW(account.id());
ASSERT_EQ(account.id(), 400);
account.Unlock();
}

TEST(Account, balance_modification) {
Account account(345, 500);

account.Lock();

ASSERT_EQ(account.GetBalance(), 500);

account.ChangeBalance(-322);
ASSERT_EQ(account.GetBalance(), 500 - 322);

account.ChangeBalance(0);
ASSERT_EQ(account.GetBalance(), 500 - 322);

account.ChangeBalance(1500);
ASSERT_EQ(account.GetBalance(), 500 - 322 + 1500);

account.Unlock();
}

TEST(Account, modify_wothout_lock) {
Account account(22, 120);
ASSERT_EQ(account.GetBalance(), 120);

ASSERT_NO_THROW(account.GetBalance());
ASSERT_EQ(account.GetBalance(), 120);

account.Lock();
ASSERT_NO_THROW(account.GetBalance());
ASSERT_EQ(account.GetBalance(), 120);
account.Unlock();
}

TEST(Account, modify_after_unlock) {
Account account(43214, 22324);
ASSERT_EQ(account.GetBalance(), 22324);

ASSERT_THROW(account.ChangeBalance(0), std::runtime_error);
ASSERT_THROW(account.ChangeBalance(412), std::runtime_error);
ASSERT_THROW(account.ChangeBalance(-24), std::runtime_error);

account.Lock();
ASSERT_EQ(account.GetBalance(), 22324);
account.ChangeBalance(555);
ASSERT_EQ(account.GetBalance(), 22324 + 555);
account.Unlock();
}

TEST(Account, double_locking) {
Account account(43252, 123);

ASSERT_NO_THROW(account.Lock());
ASSERT_THROW(account.Lock(), std::runtime_error);

ASSERT_NO_THROW(account.Unlock());
ASSERT_NO_THROW(account.Lock());
ASSERT_NO_THROW(account.Unlock());
}

TEST(Account, double_unlocking) {
Account account(43252, 123);

ASSERT_NO_THROW(account.Unlock());
ASSERT_NO_THROW(account.Lock());

ASSERT_NO_THROW(account.Unlock());
ASSERT_NO_THROW(account.Unlock());
}
