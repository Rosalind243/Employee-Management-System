# Employee-Management-System
<img width="750" height="644" alt="image" src="https://github.com/user-attachments/assets/18e87b83-85d5-426a-8096-219f9c65c782" />

## Features

### Case 1: View Record Count  
Displays the current number of records in the Employee Database.

### Case 2: Load Data From File  
- Prompts the user to enter a file to read data from.  
- Validates each record:
  - Ensures hired date is not before birth date.
- Invalid entries are skipped with a notification.
- Displays how many valid records were stored.

### Case 3: Display All Records  
- Displays all records in the Employee Database.
- Optionally prompts user to show array index.

### Case 4: Search by IC  
- Opens a sub-menu for searching records by IC.  
- Handles invalid input and prompts the user again.

### Case 5: Search by Name  
- Allows searching by employee name via a sub-menu.  
- Validates input and handles unwanted characters.

### Case 6: Search by Email  
- Enables searching by email.
- Input is checked for formatting and invalid characters.

### Case 7: Search by Phone Number  
- Finds employees using their phone number.  
- Ensures only valid phone number inputs are accepted.

### Case 8: Insert New Employee  
User can add a new employee by entering:  
- Name  
- IC  
- Email  
- Phone Number  
- Date of Birth  
- Hired Date  

All fields are validated before storing in the database.

### Case 9: Update Existing Record  
- User enters an IC number to locate an employee.  
- If a match is found, fields like IC, email, phone, birth date, and hired date can be updated.

### Case 10: Delete Employee Record  
- User enters an IC number.  
- If a match is found, employee details are shown, and a confirmation is required to delete the record.

### Case 11: Write Data to File  
- Writes all valid employee records to an external file.

### Case 12: Quit  
- Exits the program safely.

---
