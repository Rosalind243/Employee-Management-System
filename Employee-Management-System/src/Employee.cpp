
#include "Employee.h"

#include <string>

using namespace std;

Employee::Employee() : empHiredDate (Date()), empBirthDate (Date())
{
  empName = empEmail = empIC = empPhoneNum = "";
}

Employee::Employee(const std::string& name, const std::string& email,
             const std::string& ic, const std::string& phoneNum,
             const Date& hiredDate, const Date& birthDate) : empHiredDate (hiredDate), empBirthDate (birthDate)
{
  empName     = name;
  empEmail    = email;
  empIC       = ic;
  empPhoneNum = phoneNum;
}

// ------------------------------------------------

const std::string& Employee::getName() const
{
  return (empName);
}

void Employee::setName(const std::string& name)
{
  empName = name;
}

// ------------------------------------------------

const std::string& Employee::getEmail() const
{
  return (empEmail);
}

void Employee::setEmail(const std::string& email)
{
  empEmail = email;
}

// ------------------------------------------------

const std::string& Employee::getIC() const
{
  return (empIC);
}

void Employee::setIC(const std::string& ic)
{
  empIC = ic;
}

// ------------------------------------------------

const std::string& Employee::getPhoneNum() const
{
  return (empPhoneNum);
}

void Employee::setPhoneNum(const std::string& phoneNum)
{
  empPhoneNum = phoneNum;
}

// ------------------------------------------------

const Date& Employee::getHiredDate() const
{
  return (empHiredDate);
}

void Employee::setHiredDate(const Date& date)
{
  empHiredDate = date;
}

// ------------------------------------------------

const Date& Employee::getBirthDate() const
{
  return (empBirthDate);
}

void Employee::setBirthDate(const Date& date)
{
  empBirthDate = date;
}

// ------------------------------------------------
