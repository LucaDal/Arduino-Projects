CREATE TABLE utenti (
    email varchar(191) PRIMARY KEY,
    nome varchar(255),
    cognome varchar(255),
    password varchar(255)
);

CREATE TABLE prodotti (
    cod_prodotto varchar(5) PRIMARY KEY,
    email varchar(191),
    tipologia varchar(255),
    FOREIGN KEY (email) REFERENCES utenti (email)
);

CREATE TABLE term_state (
    id int AUTO_INCREMENT PRIMARY KEY,
    ora_salvataggio time,
    temp decimal(5,2),
    cod_prodotto varchar(5),
    FOREIGN KEY (cod_prodotto) REFERENCES prodotti (cod_prodotto)
);

CREATE TABLE plant_state (
    id int AUTO_INCREMENT PRIMARY KEY,
    ora_salvataggio time,
    temp_aria decimal(5,2),
    temp_terreno decimal(5,2),
    umid_aria decimal(3,0),
    umid_terreno decimal(3,0),
    cod_prodotto varchar(5),
    FOREIGN KEY (cod_prodotto) REFERENCES prodotti (cod_prodotto)
);

CREATE TABLE gestione_plant (
    cod_prodotto varchar(5) PRIMARY KEY,
    sec_to_water decimal(5,0),
    umid_to_water decimal(3,0),
    ml_to_give decimal(5,0),
    ora_accensione_luci time,
    FOREIGN KEY (cod_prodotto) REFERENCES prodotti (cod_prodotto)
);

CREATE TABLE gestione_term (
    cod_prodotto varchar(5) PRIMARY KEY,
    ora_a1 time,
    ora_s1 time,
    ora_a2 time,
    ora_s2 time,
    temp decimal(5,2),
    FOREIGN KEY (cod_prodotto) REFERENCES prodotti (cod_prodotto)
);

codici prodotti : 
8pklP   term
oJd4K	plant
A4Rqf
2ty6a

dalessandroluca1@gmail.com