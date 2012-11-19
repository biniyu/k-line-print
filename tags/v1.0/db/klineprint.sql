-- MySQL dump 10.13  Distrib 5.5.27, for Win32 (x86)
--
-- Host: localhost    Database: klineprint
-- ------------------------------------------------------
-- Server version	5.5.27

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `1min`
--

DROP TABLE IF EXISTS `1min`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `1min` (
  `symbol` varchar(10) NOT NULL,
  `month` varchar(10) NOT NULL,
  `time` datetime DEFAULT NULL,
  `open` int(11) DEFAULT NULL,
  `close` int(11) DEFAULT NULL,
  `high` int(11) DEFAULT NULL,
  `low` int(11) DEFAULT NULL,
  `vol` int(11) DEFAULT NULL,
  `interest` int(11) DEFAULT NULL,
  PRIMARY KEY (`symbol`,`month`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `pass` varchar(20) DEFAULT NULL,
  `balance` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `contract`
--

DROP TABLE IF EXISTS `contract`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `contract` (
  `symbol` varchar(10) NOT NULL COMMENT '品种代码',
  `month` varchar(10) NOT NULL COMMENT '合约月份(02, X10, Y12)',
  `fee` int(11) DEFAULT NULL COMMENT '交易所手续费',
  `margin` int(11) DEFAULT NULL COMMENT '保证金比率(0-100)',
  PRIMARY KEY (`symbol`,`month`),
  KEY `symbol_idx` (`symbol`),
  CONSTRAINT `symbol` FOREIGN KEY (`symbol`) REFERENCES `variety` (`symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `day`
--

DROP TABLE IF EXISTS `day`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `day` (
  `symbol` varchar(10) NOT NULL DEFAULT '品种',
  `month` varchar(10) NOT NULL DEFAULT '月份',
  `date` date DEFAULT NULL,
  `open` int(11) DEFAULT NULL,
  `close` int(11) DEFAULT NULL,
  `high` int(11) DEFAULT NULL,
  `low` int(11) DEFAULT NULL,
  `vol` int(11) DEFAULT NULL,
  `interest` int(11) DEFAULT NULL,
  PRIMARY KEY (`symbol`,`month`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='日线数据';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `position`
--

DROP TABLE IF EXISTS `position`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `position` (
  `account` int(11) NOT NULL,
  `time` datetime NOT NULL,
  `contract` varchar(45) DEFAULT NULL,
  `slot` int(11) DEFAULT NULL,
  `price` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `record`
--

DROP TABLE IF EXISTS `record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `record` (
  `account` int(11) NOT NULL,
  `realtime` datetime NOT NULL,
  `simutime` datetime NOT NULL,
  `contract` varchar(45) NOT NULL,
  `buy` char(1) NOT NULL,
  `open` char(1) NOT NULL,
  `price` int(11) NOT NULL,
  `slot` int(11) NOT NULL,
  `fee` int(11) NOT NULL,
  `profit` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tick`
--

DROP TABLE IF EXISTS `tick`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tick` (
  `symbol` char(10) NOT NULL COMMENT '合约名',
  `month` char(10) NOT NULL COMMENT '月份',
  `time` datetime NOT NULL COMMENT '分笔数据时间',
  `millisec` int(11) NOT NULL COMMENT '毫秒级',
  `price` int(11) DEFAULT NULL,
  `vol` int(11) DEFAULT NULL,
  `interest` int(11) DEFAULT NULL,
  `b1price` int(11) DEFAULT NULL,
  `b1vol` int(11) DEFAULT NULL,
  `b2price` int(11) DEFAULT NULL,
  `b2vol` int(11) DEFAULT NULL,
  `b3price` int(11) DEFAULT NULL,
  `b3vol` int(11) DEFAULT NULL,
  `s1price` int(11) DEFAULT NULL,
  `s1vol` int(11) DEFAULT NULL,
  `s2price` int(11) DEFAULT NULL,
  `s2vol` int(11) DEFAULT NULL,
  `s3price` int(11) DEFAULT NULL,
  `s3vol` int(11) DEFAULT NULL,
  `bs` char(1) DEFAULT NULL,
  PRIMARY KEY (`symbol`,`month`,`time`,`millisec`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `variety`
--

DROP TABLE IF EXISTS `variety`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `variety` (
  `name` varchar(20) DEFAULT NULL COMMENT '合约名(铜,橡胶)',
  `symbol` varchar(10) NOT NULL COMMENT '合约代码(cu,ru)',
  `contract_size` int(11) DEFAULT NULL COMMENT '合约规模(单位/手)',
  `tick_size` int(11) DEFAULT NULL COMMENT '最小变动价位',
  `object_unit` varchar(5) DEFAULT NULL COMMENT '货物单位(吨, 公斤)',
  `price_unit` varchar(5) DEFAULT NULL COMMENT '报价单位',
  PRIMARY KEY (`symbol`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-10-12 10:57:45
