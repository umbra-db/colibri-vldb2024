create table customer
(
    custid          int primary key,
    companyid       int,
    gender          char(6),
    name            char(15),
    age             int,
    phone           char(11),
    province        char(15),
    city            char(15),
    loan_balance    real,
    saving_credit   int,
    checking_credit int,
    loan_credit     int,
    isblocked       int,
    created_date    date
) with (%%relationConfig%%);

create table company
(
    companyid       int primary key,
    name            varchar,
    category        varchar,
    staff_size      int,
    loan_balance    real,
    phone           char(11),
    province        char(15),
    city            char(15),
    saving_credit   int,
    checking_credit int,
    loan_credit     int,
    isblocked       int,
    created_date    date
) with (%%relationConfig%%);

create table savingaccount
(
    accountid int primary key,
    userid    int,
    balance   real,
    isblocked int,
    timestamp timestamp
) with (%%relationConfig%%);

create table checkingaccount
(
    accountid int primary key,
    userid    int,
    balance   real,
    isblocked int,
    timestamp timestamp
) with (%%relationConfig%%);

create table transfer
(
    id        bigint primary key,
    sourceid  int,
    targetid  int,
    amount    real,
    type      char(10),
    timestamp timestamp
) with (%%relationConfig%%);

create table checking
(
    id        bigint primary key,
    sourceid  int,
    targetid  int,
    amount    real,
    type      char(10),
    timestamp timestamp
) with (%%relationConfig%%);

create table loanapps
(
    id          bigint primary key,
    applicantid int,
    amount      real,
    duration    int,
    status      char(12),
    timestamp   timestamp
) with (%%relationConfig%%);

create table loantrans
(
    id                 bigint primary key,
    applicantid        int,
    appid              int,
    amount             real,
    status             char(12),
    timestamp          timestamp,
    duration           int,
    contract_timestamp timestamp,
    delinquency        int
) with (%%relationConfig%%);
