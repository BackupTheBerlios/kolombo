BEGIN TRANSACTION;
CREATE TABLE concursantes (
   concursanteID INTEGER PRIMARY KEY UNIQUE,
   concursoID INTEGER,
   palomaID INTEGER,
   modalidad CHAR,
   comprobada TIME,
   velocidad FLOAT DEFAULT 0.0
);
CREATE TABLE concursos (
   concursoID INTEGER PRIMARY KEY UNIQUE,
   sueltaID INTEGER,
   diaSuelta DATE,
   horaSuelta TIME
);
CREATE TABLE nacionalidad (
   nacionalidadID INTEGER PRIMARY KEY UNIQUE,
   iniciales VARCHAR (8),
   descripcion VARCHAR (1024)
);
INSERT INTO nacionalidad VALUES(1,'ESP','Española');
INSERT INTO nacionalidad VALUES(5,'UK','Reino Unido juntito');
INSERT INTO nacionalidad VALUES(6,'FCC','FEDERACION COLOMBOFILA CUBANA');
INSERT INTO nacionalidad VALUES(7,'BELG','BELGICA');
INSERT INTO nacionalidad VALUES(8,'NL','HOLANDA');
INSERT INTO nacionalidad VALUES(9,'GB','GRAN BRETAÑA');
INSERT INTO nacionalidad VALUES(10,'PORTU','PORTUGAL');
INSERT INTO nacionalidad VALUES(11,'SU','ESECOCIA');
CREATE TABLE ojo (
   ojoID INTEGER PRIMARY KEY UNIQUE,
   nombre VARCHAR (16),
   descripcion VARCHAR (1024)
);
INSERT INTO ojo VALUES(4,'Blanco','Blanco');
INSERT INTO ojo VALUES(5,'Rojo','Rojo');
INSERT INTO ojo VALUES(6,'Negro','Negro');
INSERT INTO ojo VALUES(7,'Vinoso','Vinoso');
INSERT INTO ojo VALUES(8,'Amarillo','Amarillo');
INSERT INTO ojo VALUES(9,'Manchado','Manchado');
INSERT INTO ojo VALUES(10,'Fresa','Fresa');
INSERT INTO ojo VALUES(11,'Bandera','Amarillo y rojo');
INSERT INTO ojo VALUES(12,'Marron','Marron');
INSERT INTO ojo VALUES(13,'gris','grisaceo');
INSERT INTO ojo VALUES(14,'No definido','Color no definido');
create table paloma (
   palomaID INTEGER PRIMARY KEY UNIQUE,
   anyo INTEGER,
   anilla VARCHAR (10),
   nacionalidadID INTEGER,
   sexo BOOLEAN,
   madreID INTEGER,
   padreID INTEGER,
   plumajeID integer,
   ojoID INTEGER,
   estado CHAR,
nombre VARCHAR (255),
nota VARCHAR (1024)
);
INSERT INTO paloma VALUES(1,0000,0000000000,1,1,1,2,30,8,'R','','');
INSERT INTO paloma VALUES(2,0000,0000000000,1,0,1,2,1,11,'D','','');
CREATE TABLE parejas (
        parejaID INTEGER PRIMARY KEY UNIQUE,
        madreID INTEGER,
        padreID INTEGER,
        nombre VARCHAR (16),
        nota VARCHAR (1024)
);
CREATE TABLE plumaje (
   plumajeID INTEGER PRIMARY KEY UNIQUE,
   nombre VARCHAR (16),
   descripcion VARCHAR (1024)
);
INSERT INTO plumaje VALUES(1,'no definido','no definido');
INSERT INTO plumaje VALUES(5,'Rodado','con pintas blancas');
INSERT INTO plumaje VALUES(11,'Azul','Azul');
INSERT INTO plumaje VALUES(12,'Azul pinto','Azul con pintas blancas');
INSERT INTO plumaje VALUES(13,'Azul Ali','Azul con plumas blancas en las alas');
INSERT INTO plumaje VALUES(14,'Azul Jaspeado','Azul con motas negras sobre las alas');
INSERT INTO plumaje VALUES(15,'Rojo','Canelo');
INSERT INTO plumaje VALUES(16,'Rojo pinto','Canelo con manchas blancas');
INSERT INTO plumaje VALUES(17,'Rojo Ali','Rojo con plumas blancas en las alas');
INSERT INTO plumaje VALUES(18,'Bayo','Beige');
INSERT INTO plumaje VALUES(19,'Bayo pinto','Bayo con pintas blancas en las alas');
INSERT INTO plumaje VALUES(20,'Bayo ali','Bayo con plumas blancas en las alas');
INSERT INTO plumaje VALUES(21,'Bayo mosqueado','Bayo con pintas negras');
INSERT INTO plumaje VALUES(22,'Negro','Negro');
INSERT INTO plumaje VALUES(23,'Negro Pinto','Negro con pintas blancas');
INSERT INTO plumaje VALUES(24,'Negro ali','Negro con plumas blancas en las alas');
INSERT INTO plumaje VALUES(25,'Rojo rodado','Rojo con pintas más intensas en las alas');
INSERT INTO plumaje VALUES(26,'Azul rodado','Azul con pintas negras sobre las alas');
INSERT INTO plumaje VALUES(27,'Perla','Color grisaseo');
INSERT INTO plumaje VALUES(28,'Perla rodado','Perla con pintas de color más intenso sobre las alas');
INSERT INTO plumaje VALUES(29,'Barioles','Varios colores');
INSERT INTO plumaje VALUES(30,'Bronceado','Rodado oscuro');
INSERT INTO plumaje VALUES(31,'Bronceado ali','Bronceado con plumas blancas en las alas');
INSERT INTO plumaje VALUES(32,'Bronceado pinto ','Bronceado con pintas blancas en las alas');
INSERT INTO plumaje VALUES(33,'Blanco','Blanco');
INSERT INTO plumaje VALUES(34,'Florido','Blanco con pintas canelas');
INSERT INTO plumaje VALUES(35,'Azul jaspeado pinto','Azul co betas negras y plumas blancas');
INSERT INTO plumaje VALUES(36,'Rodado alí','Rodado con pluma blanca');
INSERT INTO plumaje VALUES(37,'Rodado pinto','Rodadfo co pintas blancas');
INSERT INTO plumaje VALUES(38,'Azul jaspeado ali','Azul co manchas negras y plumas blancas');
CREATE TABLE suelta (
   sueltaID INTEGER PRIMARY KEY UNIQUE,
   nombre VARCHAR (16),
   tipoSueltaID INTEGER,
   distancia FLOAT,
   descripcion VARCHAR (1024),
   coordx FLOAT,
   coordy FLOAT
);
INSERT INTO suelta VALUES(1,'Las Palmas',2,99536.4,'Gran Canaria',459250,3114050);
INSERT INTO suelta VALUES(2,'Morro Jable',2,99718.4,'Fuerteventura',563583,3102720);
INSERT INTO suelta VALUES(3,'Gran Tarajal',2,100466,'Fuerteventura',596100,3120700);
INSERT INTO suelta VALUES(4,'Cabo Juby',2,100251,'Costa de África',694465,3093331);
INSERT INTO suelta VALUES(5,'Arrecife',2,103398,'Lanzarote',643800,3205850);
INSERT INTO suelta VALUES(6,'Tan-Tan',2,102344,'Costa de África',881921,3114050);
INSERT INTO suelta VALUES(7,'Puerto del Rosario',2,101562,'Fuerteventura',612000,3152950);
INSERT INTO suelta VALUES(8,'Cabo Ghir',2,111677,'Costa de África',984640,3391637);
INSERT INTO suelta VALUES(9,'Essaquira',2,114690,'Costa de África',995292,3487721);
INSERT INTO suelta VALUES(10,'Safi',2,117277,'Costa de África',1045135,3558434);
INSERT INTO suelta VALUES(11,'Casablanca',2,123717,'Costa de África',1200076,3723796);
CREATE TABLE tipoSuelta (
   tipoSueltaID INTEGER PRIMARY KEY UNIQUE,
   nombre VARCHAR (16),
   descripcion VARCHAR (1024)
);
INSERT INTO tipoSuelta VALUES(2,'fondo','fondo a partir de 350km');
INSERT INTO tipoSuelta VALUES(3,'gran fondo',' a partir de 500km.');
INSERT INTO tipoSuelta VALUES(4,'velocidad','menos de 300km,');
CREATE VIEW idCompacto AS
   select palomaID as "palomaID", anyo || "-" || anilla || "-" || nacionalidad.iniciales as "idCompacto"
   from paloma, nacionalidad
   where paloma.nacionalidadID = nacionalidad.nacionalidadID
      ORDER BY paloma.anyo DESC, paloma.anilla, nacionalidad.iniciales;
CREATE VIEW listaGeneral AS
select paloma.palomaID as "palomaID", paloma.anyo as "anyo", paloma.anilla as
"anilla", nacionalidad.iniciales as "nacionalidad",
   case paloma.sexo
      when 0 then "Macho"
      when 1 then "Hembra"
   end as "sexo",
   plumaje.nombre as "plumaje", ojo.nombre as "ojo",
   case paloma.estado
      when "V" then "Vuelo"
      when "R" then "Reproduccion"
      when "D" then "Desaparecida"
   end as "estado", paloma.nombre as "nombre", madre.anyo || "-" || madre.anilla || "-" || nacionmadre.iniciales
   as "madre", padre.anyo || "-" || padre.anilla || "-" || nacionpadre.iniciales as "padre"
from paloma, nacionalidad, plumaje, ojo, paloma madre, paloma padre,
nacionalidad nacionpadre, nacionalidad nacionmadre
where paloma.nacionalidadID = nacionalidad.nacionalidadID
   and paloma.plumajeID = plumaje.plumajeID and
   paloma.ojoID = ojo.ojoID and madre.palomaID = paloma.madreID and
   padre.palomaID = paloma.padreID and nacionpadre.nacionalidadID =
padre.nacionalidadID
        and nacionmadre.nacionalidadID = madre.nacionalidadID and
paloma.palomaID != 1
        and paloma.palomaID != 2 ORDER BY paloma.anyo DESC, paloma.anilla,
        nacionalidad.iniciales;
create view vistaConcursantes as
   select b.concursoID as concursoID,
   s.distancia as distancia,
   a.comprobada as comprobada,
   l.anyo as anyo,
   l.anilla as anilla,
   l.nacionalidad as nacionalidad,
   case a.modalidad
      when "D" then "Designada"
      when "S" then "Serie"
      when "C" then "Concurso"
      when "F" then "Fuera de concurso"
      when "X" then "Serie y designada"
   end as modalidad,
   a.velocidad as velocidad,
   a.palomaID as palomaID
   from concursantes a, concursos b, suelta s, listaGeneral l
   where a.concursoID = b.concursoID and s.sueltaID = b.sueltaID and a.palomaID = l.palomaID;
create view vistaConcursos as
   select c.concursoID as concursoID,
         c.diaSuelta as diaSuelta,
         c.horaSuelta as horaSuelta,
         s.nombre as nombre,
         s.distancia as distancia,
         "[" || c.concursoID || "] (" || c.diaSuelta || ") - " || s.nombre || " - " || s.distancia || "Km" as descripcion
         from concursos c, suelta s
         where c.sueltaID = s.sueltaID;
create view vistaParejas as
   select parejaID, nombre,
   A.idCompacto as "madre",
   B.idCompacto as "padre", nota
   from parejas, idCompacto A, idCompacto B
   where A.palomaID = parejas.madreID and B.palomaID = parejas.padreID order by parejaID;
create view vistaSueltas as
   select sueltaID, A.nombre as "nombre", B.nombre as "tipoSuelta", distancia, A.descripcion as "descripcion",
   coordx as "X", coordy as "Y"
   from suelta A, tipoSuelta B
   where A.tipoSueltaID = B.tipoSueltaID;
CREATE TRIGGER delete_paloma BEFORE DELETE ON paloma
FOR EACH ROW
BEGIN
        SELECT RAISE (ABORT, "Error paloma con descendencia, no la puede borrar")
        FROM paloma
        WHERE madreID = old.palomaID OR padreID = old.palomaID;
END;
CREATE TRIGGER insert_concursantes BEFORE INSERT ON concursantes
FOR EACH ROW
BEGIN
        SELECT RAISE (ABORT, "Error, esa paloma ya ha sido seleccionada para viajar")
        FROM concursantes
        WHERE concursoID = new.concursoID and palomaID = new.palomaID;
        SELECT RAISE (ABORT, "Error, ya hay un designado en este concurso")
        FROM concursantes
        WHERE concursoID = new.concursoID and modalidad = new.modalidad and modalidad = "D";
        SELECT RAISE (ABORT, "Error, ya hay 3 palomas en la serie")
        FROM concursantes
        WHERE concursoID = new.concursoID
            and (modalidad = "S" or modalidad = "X")
            and (new.modalidad = "S" or new.modalidad = "X")
        GROUP BY concursoID
        HAVING count (concursoID) = 3;
        SELECT RAISE (ABORT, "Error, ya hay 5 palomas a concurso")
        FROM concursantes
        WHERE concursoID = new.concursoID
            and (modalidad = "D" or modalidad = "S" or modalidad = "C")
            and new.modalidad != "F"
        GROUP BY concursoID
        HAVING count (concursoID) = 5;
END;
CREATE TRIGGER insert_dni BEFORE INSERT ON paloma
FOR EACH ROW
BEGIN
        SELECT  RAISE (ABORT, "Existe una paloma con esos datos.")
        FROM paloma
        WHERE anyo = new.anyo AND anilla = new.anilla AND nacionalidadID =
        new.nacionalidadID;
END;
CREATE TRIGGER insert_parejas BEFORE INSERT ON parejas
FOR EACH ROW
BEGIN
        SELECT RAISE (ABORT, "Error, ya existe esta pareja en la base de datos")
        FROM parejas
        WHERE madreID = new.madreID and padreID = new.padreID;
END;
CREATE TRIGGER update_concursantes BEFORE UPDATE ON concursantes
FOR EACH ROW
BEGIN
        SELECT RAISE (ABORT, "Error, ya hay designada")
        FROM concursantes
        WHERE concursoID in
            (SELECT concursoID
               FROM concursantes
               WHERE concursoID = new.concursoID and modalidad = "D" or modalidad = "X")
            and (modalidad != "D" and modalidad != "X") and palomaID = new.palomaID
            and (new.modalidad = "X" or new.modalidad = "D");
        SELECT RAISE (ABORT, "Error, ya hay 5 palomas a concurso")
        FROM concursantes
         WHERE concursoID in
            (SELECT concursoID
               FROM concursantes
               WHERE concursoID = new.concursoID and (modalidad = "D" or modalidad = "S" or modalidad = "C")
               GROUP BY concursoID
               HAVING count (concursoID) = 5)
            and palomaID = new.palomaID and modalidad = "F" and new.modalidad != "F";
        SELECT RAISE (ABORT, "Error, ya hay 3 palomas en la serie")
        FROM concursantes
        WHERE concursoID in
            (SELECT concursoID
               FROM concursantes
               WHERE concursoID = new.concursoID and (modalidad = "S" or modalidad = "X")
               GROUP BY concursoID
               HAVING count (concursoID) = 3)
            and (modalidad != "S" and modalidad != "X") and (new.modalidad = "X" or new.modalidad = "S")
            and palomaID = new.palomaID;
END;
CREATE TRIGGER update_dni BEFORE UPDATE OF anyo, anilla, nacionalidad ON paloma
FOR EACH ROW
BEGIN
        SELECT  RAISE (ABORT, "Existe una paloma con esos datos.")
        FROM paloma
        WHERE anyo = new.anyo AND anilla = new.anilla AND nacionalidadID =
        new.nacionalidadID AND palomaID != old.palomaID;
END;
CREATE TRIGGER update_sexo BEFORE UPDATE OF sexo ON paloma
FOR EACH ROW
BEGIN
        SELECT RAISE (ABORT, "Paloma con descendencia, no se le puede cambiar el sexo.")
        FROM paloma
        WHERE madreID = old.palomaID OR padreID = old.palomaID;
END;
COMMIT;
