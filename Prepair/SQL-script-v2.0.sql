----------------------------------------------------[create_database]----------------------------------------------------
--создать базу данных

--    encoding = 'UTF8'
-- create database libraryIS
--    connection limit = -1;
--    owner = admin



--запустить оставшуюся часть
begin;


create extension pgcrypto;


----------------------------------------------------[create_tables]----------------------------------------------------
   
   
create table public."reader_ticket" (
    "TicketID" serial,
    "FirstName" text not null,
    "LastName" text not null,
    "Patronymic" text,
    "BirthDate" date not null,
    "login" text not null unique,
    "password" text not null,
    "role" text not null default 'user',
    constraint "ticketid-pk" primary key ("TicketID") );

--

create index index_reader_ticket on public."reader_ticket"("login", "password", "role");

--


create table public."genre" (
    "GenreID" smallserial,
    "GenreName" text NOT NULL,
    constraint "genreid-pk" primary key ("GenreID") );


--


create table public."book" (
    "BookID" serial,
    "GenreID" smallserial,
    "BookCount" smallint not null,
    "BookName" text NOT NULL,
    "BookAuthor" text,
    "BookPublishDate" smallint,
    constraint "bookid-pk" primary key ("BookID"),
    constraint "genreid-fk" foreign key ("GenreID")
        references public."genre" ("GenreID") match simple );
    

--


create table public."record" (
    "RecordID" bigserial,
    "TicketID" serial,
    "BookID" serial,
    "BookTake" date not null,
    "BookRetern" date,
    constraint "recordid-pk" primary key ("RecordID"),
    constraint "tiketid-fk" foreign key ("TicketID")
        references public."reader_ticket" ("TicketID") match simple,
    constraint "bookid-fk" foreign key ("BookID")
        references public."book" ("BookID") match simple );


--


create view full_info_of_book as
    select "BookName", "BookAuthor", "GenreName", "BookPublishDate", "BookCount", "BookID"
    from book left join genre on book."GenreID" = genre."GenreID";


----------------------------------------------------[create_functions_and_procedures]----------------------------------------------------


create procedure public.set_full_info_of_book(BookID int, BookName text, BookAuthor text, GenreName text, BookPublishDate int, BookCount int)
    language sql as $$
	    update public.book set "BookName" = $2, "BookAuthor" = $3, "BookPublishDate" = $5, "BookCount" = $6,
            "GenreID" = (select "GenreID" from genre where genre."GenreName" = $4)
            where "BookID" = $1;
$$;


--


create function public.authorizate("login" text, "password" text)
    returns table("result" bool, "role" text)
    language sql as $$
	    select ("password" = crypt($2, "password")) as "result", "role" from public.reader_ticket
            where ("password" = crypt($2, "password")) and "login" = $1 ;
$$;


--


create function public.registration(FirstName text, LastName text, Patronymic text, BithDate date, "login" text, "password" text)
    returns bool
    language plpgsql as $$
    begin
        if (select 1 from public.reader_ticket rt where rt.login = $5) then
            return false;
        end if;
        insert into public.reader_ticket values(default, $1, $2, $3, $4, $5, crypt($6, gen_salt('bf')), 'user');
        return true;
    end;
$$;


--


create procedure add_book(BookName text, BookAuthor text, GenreName text, BookPublishDate int, BookCount int)
    language sql as $$
        insert into public.book ("BookName", "BookAuthor", "BookPublishDate", "BookCount", "GenreID")
            values ( $1, $2, $4, $5, (select "GenreID" from genre where genre."GenreName" = $3));
$$;


--


create function print_taken_books_by_user_login("login" text)
    returns table("BookName" text, "BookAuthor" text, "GenreName" text, "BookTake" date)
    language sql as $$
        select "BookName", "BookAuthor", "GenreName", "BookTake" from reader_ticket
            left outer join (select * from record left join book on record."BookID" = book."BookID")
                as undqer on undqer."TicketID" = reader_ticket."TicketID"
            left outer join genre on genre."GenreID" = undqer."GenreID"
            where login = $1 and "BookRetern" is null;
$$;


--


create function print_retuns_books_by_user_login("login" text)
    returns table("BookName" text, "BookAuthor" text, "GenreName" text, "BookTake" date, "BookRetern" date)
    language sql as $$
        select "BookName", "BookAuthor", "GenreName", "BookTake", "BookRetern" from reader_ticket
            left outer join (select * from record left join book on record."BookID" = book."BookID")
                as undqer on undqer."TicketID" = reader_ticket."TicketID"
            left outer join genre on genre."GenreID" = undqer."GenreID"
            where login = $1 and "BookRetern" is not null;
$$;


--


create function set_login("old_login" text, "current_password" text, "new_login" text)
    returns bool
    language plpgsql as $$
    begin
        if (select (rt."password" = crypt($2, rt."password")) as "result" from public.reader_ticket rt
            where (rt."password" = crypt($2, rt."password")) and rt."login" = $1) then
                update reader_ticket set "login" = $3 where "login" = $1;
                return true;
        else
            return false;
        end if;
    end; 
$$;


--


create function set_password("login" text, "current_password" text, "new_password" text)
    returns bool
    language plpgsql as $$
    begin
        if (select (rt."password" = crypt($2, rt."password")) as "result" from public.reader_ticket rt
            where (rt."password" = crypt($2, rt."password")) and rt."login" = $1) then
                update reader_ticket set "password" = crypt($3, gen_salt('bf')) where public.reader_ticket.login = $1;
                return true;
        else
            return false;
        end if;
    end;
$$;


----------------------------------------------------[filling_in_tables]----------------------------------------------------


insert into public.genre 
values (default, 'Роман'),
		(default, 'Эпопея'),
		(default, 'Фантастика'),
		(default, 'Детектив'),
		(default, 'Комедия');

--

insert into public.reader_ticket ("FirstName", "LastName", "Patronymic", "BirthDate", "login", "password")
values ('Иван', 'Иванов', 'Иванович', '2000-12-12', 'ivan', crypt('123456', gen_salt('bf'))),
		('Петя', 'Петров', NULL, '1999-10-06', 'petya', crypt('123456', gen_salt('bf'))),
        ('Администратор', 'Администраторов', 'Администратович', '1900-01-01', 'admin', crypt('123456', gen_salt('bf')));
        
--
	
insert into public.book
values (default, 1, random()*10, 'Фотографов с рук не кормить!', 'Надежда Мамаева', 2023),
		(default, 1, random()*100, 'Тихий Дон', 'Михаил Шолохов', 2014),
		(default, 1, random()*10, 'Потерянные сердца', 'Эми Хармон', 2021),
		(default, 2, random()*100, 'Петр Первый', 'Алексей Толстой', 2008),
		(default, 2, random()*10, 'Сага о Форсайтах', 'Джон Голсуорси', 2020),
		(default, 3, random()*100, '451 градус по Фаренгейту', 'Рэй Брэдбери', 2018),
		(default, 4, random()*100, 'Санаторий «Седьмое небо', 'Полина Луговцова', 2020),
		(default, 5, random()*10, 'Упал. Очнулся. Папа!', 'Янина Логвин', 2022);
		
--
		
insert into public.record
values (default, 1, 6, '2022-08-08', '2023-02-21'),
		(default, 1, 1, '2023-02-21', NULL),
		(default, 2, 4, '2023-02-21', NULL);
	

GRANT INSERT, SELECT, DELETE, UPDATE ON TABLE public.book TO "admin";
GRANT INSERT, SELECT, DELETE, UPDATE ON TABLE public.genre TO "admin";
GRANT INSERT, SELECT, DELETE, UPDATE ON TABLE public.reader_ticket TO "admin";
GRANT INSERT, SELECT, DELETE, UPDATE ON TABLE public.record TO "admin";
GRANT SELECT ON TABLE public.full_info_of_book TO "admin";
GRANT INSERT, SELECT ON TABLE public.reader_ticket TO "guest";
GRANT SELECT ON TABLE public.full_info_of_book TO "guest";
GRANT SELECT ON TABLE public.genre TO "guest";
GRANT SELECT ON TABLE public.book TO "guest";
GRANT SELECT, UPDATE ON TABLE public.full_info_of_book TO "user";
GRANT SELECT, UPDATE ON TABLE public.reader_ticket TO "user";
GRANT SELECT ON TABLE public.record TO "user";
GRANT SELECT ON TABLE public.genre TO "user";
GRANT SELECT ON TABLE public.book TO "user";


commit;