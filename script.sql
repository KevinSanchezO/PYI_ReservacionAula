CREATE DATABASE IF NOT EXISTS DB_Reservacion_Aulas;

USE DB_Reservacion_Aulas;


CREATE TABLE Aulas (
nombreAula VARCHAR(90) NOT NULL PRIMARY KEY,
capacidad int
);

CREATE TABLE Cursos (
codCarrera int,
codCurso varchar(6) NOT NULL PRIMARY KEY,
nombre varchar(90)
);

CREATE TABLE Profesores(
cedula int NOT NULL PRIMARY KEY,
nombreProfesor varchar(90)
);

CREATE TABLE CursosPorPeriodo(
codCurso varchar(6) NOT NULL,
year int NOT NULL,
periodo int NOT NULL,
grupo int NOT NULL,
cedulaProfesor int,
cantEstudiantes int,
PRIMARY KEY (codCurso, year, periodo, grupo),
FOREIGN KEY (codCurso) REFERENCES Cursos (codCurso),
FOREIGN KEY (cedulaProfesor) REFERENCES Profesores (cedula)
);

CREATE TABLE ReservacionAulas(
codReservacion int NOT NULL PRIMARY KEY auto_increment,
fecha varchar(30)NOT NULL,
horaInicio varchar(30)NOT NULL,
horaFin varchar(30)NOT NULL,
codCurso varchar(6)NOT NULL,
periodo int NOT NULL,
anio int NOT NULL,
grupo int NOT NULL,
nombreAula varchar(10),
FOREIGN KEY (codCurso) REFERENCES Cursos (codCurso),
FOREIGN KEY (nombreAula) REFERENCES Aulas (nombreAula)
);


INSERT INTO Cursos(codCarrera, codCurso, nombre) VALUES
(1, 'IC1111', 'Introduccion a la programacion'),
(2, 'IC2222', 'Taller de programacion'),
(3, 'IC3333', 'Estructura de datos'),
(4, 'IC4444', 'Arquitectura de computadores'),
(5, 'IC5555', 'Analisis de Algoritmos'),
(6, 'IC6666', 'Fundamentos de computadores'),
(7, 'IC7777', 'Bases de datos'),
(8, 'IC8888', 'Lenguajes de programacion'),
(9, 'IC9999', 'Requerimientos de software'),
(10, 'MA1111', 'Matematica general'),
(11, 'MA2222', 'Matematica discreta'),
(12, 'MA3333', 'Calculo'),
(13, 'MA4444', 'Algebra'),
(14, 'MA5555', 'Estadistica'),
(15, 'MA6666', 'Probabilidad'),
(16, 'MA7777', 'Estadistica'),
(17, 'MA8888', 'Estadistica'),
(18, 'CS1111', 'Ambiente humano'),
(19, 'CS2222', 'Filosofia'),
(20, 'CS3333', 'Historia');
