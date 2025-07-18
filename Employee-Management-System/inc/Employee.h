

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Date.h"

#include <string>

const int EMP_IDX_LENGTH = 8;
const int EMP_NAME_LENGTH = 36;
const int EMP_EMAIL_LENGTH = 36;
const int EMP_IC_LENGTH = 11;
const int EMP_TEL_LENGTH = 16;
const int EMP_DATE_LENGTH = 16;
const std::string DATE_DELIMITER = "/";
const std::string RECORD_DELIMITER = ",";
const std::string CSV_FILE_HEADER_LINE = "Idx,Name,Email,IC,PhoneNum,HireDate,BirthDate";

class Employee 
{
private:
    std::string empName;      // e.g., John Doe Anderson
    std::string empEmail;     // e.g., johndoe@virtualworld.com
    std::string empIC;        // e.g., S8765432G
    std::string empPhoneNum;  // e.g., (65) 87654321

    Date empHiredDate;
    Date empBirthDate;

public:
    Employee ();
    Employee(const std::string& name, const std::string& email,
             const std::string& ic, const std::string& phoneNum,
             const Date& hiredDate, const Date& birthDate);

    // Getters and setters
    const std::string& getName() const;
    void setName(const std::string& name);

    const std::string& getEmail() const;
    void setEmail(const std::string& email);

    const std::string& getIC() const;
    void setIC(const std::string& ic);

    const std::string& getPhoneNum() const;
    void setPhoneNum(const std::string& phoneNum);

    const Date& getHiredDate() const;
    void setHiredDate(const Date& date);

    const Date& getBirthDate() const;
    void setBirthDate(const Date& date);

};

#endif

