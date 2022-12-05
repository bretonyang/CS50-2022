-- Keep a log of any SQL queries you execute as you solve the mystery.

-- TO understand what's in the database.
.schema


-- To find out the description of a crime scene on the given date and street.
-- Get info: crime happened at 10:15am at chamberlin street courthouse
--           three witnesses mention courthouse in their interviews
SELECT description FROM crime_scene_reports WHERE year = 2020 AND month = 7 AND day = 28 AND street LIKE "Chamberlin street";


-- To see the interview transcripts that mentions "courthouse".
-- Get info: Ruth:  within 10 mins of the theft, the thief get into a car and left courthouse parking lot.
--           Eugene: that morning, saw theif withdrawing money from ATM on Fifer Street.
--           Raymond: Before theif left courthouse, they called someone and talked less than 1 min
--                    theif took the earliest flight tomorrow(7/29), person on the other end purchase flight ticket.
SELECT name, transcript FROM interviews WHERE transcript LIKE "%courthouse%" AND month = 7 AND day = 28;


-- To get the license plate of the car that exit during that time
-- license plate: 5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55
SELECT activity, license_plate FROM courthouse_security_logs WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25;


-- To find out who withdraw money at atm on fifer street
SELECT account_number, amount FROM atm_transactions WHERE atm_location LIKE "fifer street"
AND transaction_type = "withdraw" AND month = 7 AND day = 28;


-- Get the phone numbers of people who called at that day and less than 1 min
SELECT caller, receiver FROM phone_calls WHERE month = 7 AND day = 28 AND duration < 60;


-- Find the airport id and flight id for the earliest flight on 7/29
-- the theif left from airport_id 8 to airport_id 4 via flight_id 36 at 8:20 on 7/29
SELECT hour, minute, id, origin_airport_id, destination_airport_id FROM flights WHERE month = 7 AND day = 29 ORDER BY hour;


-- Find out where the theif escaped to: London
SELECT abbreviation, full_name, city FROM airports WHERE id = 4;


-- Find out who took the flight with flight_id 36
SELECT passport_number FROM passengers WHERE flight_id = 36;


-- Combine flight_id and license_plate and phone_number to get the suspects' info :
SELECT * FROM people
WHERE passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = 36)
AND license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25)
AND phone_number IN (SELECT caller FROM phone_calls WHERE month = 7 AND day = 28 AND duration < 60);


-- Find out who was withdrawing at 7/28 morning
SELECT * FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE person_id IN (
SELECT person_id FROM bank_accounts
WHERE account_number IN (
SELECT account_number FROM atm_transactions WHERE atm_location LIKE "fifer street"
AND transaction_type = "withdraw" AND month = 7 AND day = 28));

-- Combine the two queries above we find out the info of the theif: Ernest

SELECT * FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE person_id IN (
SELECT person_id FROM bank_accounts
WHERE account_number IN (
SELECT account_number FROM atm_transactions WHERE atm_location LIKE "fifer street"
AND transaction_type = "withdraw" AND month = 7 AND day = 28))
AND passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = 36)
AND license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25)
AND phone_number IN (SELECT caller FROM phone_calls WHERE month = 7 AND day = 28 AND duration < 60);


-- since we have the theif's phone number we can track back to see who the receiver is in the call at courthouse
-- So we find out the accomplice was Berthold
SELECT name FROM people WHERE phone_number = (
SELECT receiver FROM phone_calls WHERE month = 7 AND day = 28
AND duration < 60 AND caller = "(367) 555-5533");

