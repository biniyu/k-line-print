-- MySQL dump 10.13  Distrib 5.6.10, for Win32 (x86)
--
-- Host: localhost    Database: toptrader
-- ------------------------------------------------------
-- Server version	5.6.10-log

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
  `code` varchar(20) NOT NULL,
  `time` datetime DEFAULT NULL,
  `open` int(11) DEFAULT NULL,
  `close` int(11) DEFAULT NULL,
  `high` int(11) DEFAULT NULL,
  `low` int(11) DEFAULT NULL,
  `vol` int(11) DEFAULT NULL,
  `interest` int(11) DEFAULT NULL,
  PRIMARY KEY (`code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account` (
  `account` int(11) NOT NULL AUTO_INCREMENT COMMENT '账号',
  `pass` varchar(20) DEFAULT NULL COMMENT '密码',
  `balance` float DEFAULT NULL COMMENT '静态权益(+动态盈亏=动态权益)',
  `available` float DEFAULT NULL COMMENT '可用资金',
  `currency` varchar(20) DEFAULT NULL COMMENT '货币',
  `lastbalance` float DEFAULT NULL COMMENT '上日结存',
  `buyfreeze` float DEFAULT NULL COMMENT '买入冻结',
  `sellfreeze` float DEFAULT NULL COMMENT '卖出冻结',
  `buymargin` float DEFAULT NULL COMMENT '买入保证金',
  `sellmargin` float DEFAULT NULL COMMENT '卖出保证金',
  `settleprofit` float DEFAULT NULL COMMENT '平仓盈亏',
  `fee` float DEFAULT NULL COMMENT '手续费',
  PRIMARY KEY (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `day`
--

DROP TABLE IF EXISTS `day`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `day` (
  `code` varchar(20) NOT NULL DEFAULT '品种',
  `date` date DEFAULT NULL,
  `open` int(11) DEFAULT NULL,
  `close` int(11) DEFAULT NULL,
  `high` int(11) DEFAULT NULL,
  `low` int(11) DEFAULT NULL,
  `vol` int(11) DEFAULT NULL,
  `interest` int(11) DEFAULT NULL,
  PRIMARY KEY (`code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='日线数据';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `market`
--

DROP TABLE IF EXISTS `market`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `market` (
  `code` varchar(20) NOT NULL COMMENT '市场代码',
  `name` varchar(45) DEFAULT NULL COMMENT '市场名称',
  `timezone` tinyint(4) DEFAULT NULL COMMENT '时区(伦敦0, 北京+8等)',
  PRIMARY KEY (`code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `order`
--

DROP TABLE IF EXISTS `order`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `order` (
  `account` int(11) NOT NULL,
  `orderid` int(11) NOT NULL,
  `code` varchar(45) DEFAULT NULL,
  `status` varchar(45) DEFAULT NULL,
  `buy` varchar(45) DEFAULT NULL,
  `open` varchar(45) DEFAULT NULL,
  `commissionslot` int(11) DEFAULT NULL,
  `commissionprice` int(11) DEFAULT NULL,
  `settleslot` int(11) DEFAULT NULL,
  `settleprice` int(11) DEFAULT NULL,
  `unsettleslot` int(11) DEFAULT NULL,
  `commissiontime` datetime DEFAULT NULL,
  `declaretime` datetime DEFAULT NULL,
  `canceltime` datetime DEFAULT NULL,
  PRIMARY KEY (`orderid`,`account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `playback`
--

DROP TABLE IF EXISTS `playback`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `playback` (
  `account` int(11) NOT NULL COMMENT '账号',
  `play_sequence` enum('Seq','Random') DEFAULT NULL COMMENT '顺序播放，随机播放',
  `play_speed` int(11) DEFAULT NULL,
  `date_range_type` enum('All','StartEnd','Recent') DEFAULT NULL COMMENT 'All-所有日期的数据 \nStartEnd-在指定的日期内\nRecent-在最近几个月内',
  `date_range_start` date DEFAULT NULL COMMENT '起始日期',
  `date_range_end` date DEFAULT NULL COMMENT '结束日期',
  `date_range_recent` int(11) DEFAULT NULL COMMENT '当前日期几个月内',
  `time_start` time DEFAULT NULL COMMENT '开始时间',
  `time_end` time DEFAULT NULL COMMENT '结束时间',
  `weekday_mon` char(1) DEFAULT NULL,
  `weekday_tue` char(1) DEFAULT NULL,
  `weekday_wed` char(1) DEFAULT NULL,
  `weekday_thu` char(1) DEFAULT NULL,
  `weekday_fri` char(1) DEFAULT NULL,
  `cur_code` varchar(45) DEFAULT NULL,
  `cur_date` date DEFAULT NULL,
  `cur_time` time DEFAULT NULL,
  PRIMARY KEY (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='回放参数';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `position`
--

DROP TABLE IF EXISTS `position`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `position` (
  `account` int(11) NOT NULL,
  `code` varchar(45) DEFAULT NULL,
  `buyslot` int(11) DEFAULT NULL COMMENT '买持仓',
  `buyprice` int(11) DEFAULT NULL COMMENT '买均价',
  `sellslot` int(11) DEFAULT NULL COMMENT '卖持仓',
  `sellprice` int(11) DEFAULT NULL COMMENT '卖均价',
  `todaybuyslot` int(11) DEFAULT NULL COMMENT '今买持仓',
  `todaysellslot` int(11) DEFAULT NULL COMMENT '今卖持仓'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='持仓信息';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tick`
--

DROP TABLE IF EXISTS `tick`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tick` (
  `code` char(20) NOT NULL COMMENT '代码(市场+代码)\\nSH.600001, SZ.000002, SQ.CU.12, DL.A.5, USD/EUR, USD/JPY\\n',
  `time` datetime NOT NULL COMMENT '分笔数据时间',
  `usec` mediumint(9) NOT NULL COMMENT '微秒级',
  `price` int(11) DEFAULT NULL,
  `vol` int(11) DEFAULT NULL,
  `interest` int(11) DEFAULT NULL,
  `b1price` int(11) DEFAULT NULL,
  `b1vol` int(11) DEFAULT NULL,
  `s1price` int(11) DEFAULT NULL,
  `s1vol` int(11) DEFAULT NULL,
  `bs` char(1) DEFAULT NULL,
  PRIMARY KEY (`code`,`time`,`usec`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='一档买卖盘(期货)';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tick.10`
--

DROP TABLE IF EXISTS `tick.10`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tick.10` (
  `code` char(20) NOT NULL COMMENT '代码(市场+代码)\\nSH.600001, SZ.000002, SQ.CU.12, DL.A.5, USD/EUR, USD/JPY\\n',
  `time` datetime NOT NULL COMMENT '分笔数据时间',
  `usec` mediumint(9) NOT NULL COMMENT '微秒级',
  `b6price` int(11) DEFAULT NULL,
  `b6vol` int(11) DEFAULT NULL,
  `b7price` int(11) DEFAULT NULL,
  `b7vol` int(11) DEFAULT NULL,
  `b8price` int(11) DEFAULT NULL,
  `b8vol` int(11) DEFAULT NULL,
  `b9price` int(11) DEFAULT NULL,
  `b9vol` int(11) DEFAULT NULL,
  `b10price` int(11) DEFAULT NULL,
  `b10vol` int(11) DEFAULT NULL,
  `s6price` int(11) DEFAULT NULL,
  `s6vol` int(11) DEFAULT NULL,
  `s7price` int(11) DEFAULT NULL,
  `s7vol` int(11) DEFAULT NULL,
  `s8price` int(11) DEFAULT NULL,
  `s8vol` int(11) DEFAULT NULL,
  `s9price` int(11) DEFAULT NULL,
  `s9vol` int(11) DEFAULT NULL,
  `s10price` int(11) DEFAULT NULL,
  `s10vol` int(11) DEFAULT NULL,
  PRIMARY KEY (`code`,`time`,`usec`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='10档买卖盘(股票Level2)';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tick.3`
--

DROP TABLE IF EXISTS `tick.3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tick.3` (
  `code` char(20) NOT NULL COMMENT '代码(市场+代码)\\nSH.600001, SZ.000002, SQ.CU.12, DL.A.5, USD/EUR, USD/JPY\\n',
  `time` datetime NOT NULL COMMENT '分笔数据时间',
  `usec` mediumint(9) NOT NULL COMMENT '微秒级',
  `b2price` int(11) DEFAULT NULL,
  `b2vol` int(11) DEFAULT NULL,
  `b3price` int(11) DEFAULT NULL,
  `b3vol` int(11) DEFAULT NULL,
  `s2price` int(11) DEFAULT NULL,
  `s2vol` int(11) DEFAULT NULL,
  `s3price` int(11) DEFAULT NULL,
  `s3vol` int(11) DEFAULT NULL,
  PRIMARY KEY (`code`,`time`,`usec`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='3档买卖盘(股票)';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tick.5`
--

DROP TABLE IF EXISTS `tick.5`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tick.5` (
  `code` char(20) NOT NULL COMMENT '代码(市场+代码)\\nSH.600001, SZ.000002, SQ.CU.12, DL.A.5, USD/EUR, USD/JPY\\n',
  `time` datetime NOT NULL COMMENT '分笔数据时间',
  `usec` mediumint(9) NOT NULL COMMENT '微秒级',
  `b4price` int(11) DEFAULT NULL,
  `b4vol` int(11) DEFAULT NULL,
  `b5price` int(11) DEFAULT NULL,
  `b5vol` int(11) DEFAULT NULL,
  `s4price` int(11) DEFAULT NULL,
  `s4vol` int(11) DEFAULT NULL,
  `s5price` int(11) DEFAULT NULL,
  `s5vol` int(11) DEFAULT NULL,
  PRIMARY KEY (`code`,`time`,`usec`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='5档买卖盘(股票)';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `transaction`
--

DROP TABLE IF EXISTS `transaction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `transaction` (
  `account` int(11) NOT NULL,
  `realtime` datetime NOT NULL,
  `simutime` datetime NOT NULL,
  `code` varchar(45) NOT NULL,
  `orderid` int(11) DEFAULT NULL COMMENT '委托单号',
  `transid` int(11) DEFAULT NULL COMMENT '成交号',
  `buy` char(1) NOT NULL,
  `open` char(1) NOT NULL,
  `price` int(11) NOT NULL,
  `slot` int(11) NOT NULL,
  `fee` int(11) NOT NULL,
  `profit` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='成交记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `trigger`
--

DROP TABLE IF EXISTS `trigger`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `trigger` (
  `account` int(11) NOT NULL,
  `id` int(11) DEFAULT NULL COMMENT '条件单号',
  `type` enum('Time','Price') DEFAULT NULL COMMENT '条件类型(时间触发，价格触发)',
  `timetrigger` time DEFAULT NULL COMMENT '时间触发',
  `pricetrigger` float DEFAULT NULL COMMENT '价格触发',
  `priceabove` char(1) DEFAULT NULL COMMENT '价格大于/小于',
  `code` varchar(45) DEFAULT NULL,
  `open` char(1) DEFAULT NULL,
  `buy` char(1) DEFAULT NULL,
  `slot` int(11) DEFAULT NULL,
  `pricetype` enum('Market') DEFAULT NULL,
  `price` float DEFAULT NULL COMMENT '限价的价格',
  `tick` int(11) DEFAULT NULL COMMENT '追价的档位',
  `status` enum('Fired','To Fire','Cancelled') DEFAULT NULL COMMENT '触发状态',
  `firetime` datetime DEFAULT NULL COMMENT '实际触发时间',
  `commissionprice` float DEFAULT NULL COMMENT '实际报单价格',
  PRIMARY KEY (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='条件单(雷凯预埋/条件单)';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `variety`
--

DROP TABLE IF EXISTS `variety`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `variety` (
  `code` varchar(20) NOT NULL COMMENT '品种代码(含市场代码)',
  `market` varchar(10) DEFAULT NULL COMMENT '市场编码(SH,SZ,SQ,DL,ZZ等)',
  `name` varchar(20) DEFAULT NULL COMMENT '品种显示名',
  `type` enum('Stock','Future Variety','Future Contract','Currency','Warrant','Actual') DEFAULT NULL COMMENT '类型(股票, 期货品类, 期货合约, 外汇, 权证, 现货等)',
  `contract_size` int(11) DEFAULT NULL COMMENT '合约规模(单位/手)',
  `tick_size` float DEFAULT NULL COMMENT '最小变动价位',
  `object_unit` varchar(5) DEFAULT NULL COMMENT '货物单位(吨, 公斤)',
  `price_unit` varchar(5) DEFAULT NULL COMMENT '报价单位',
  `price_precision` tinyint(3) unsigned DEFAULT NULL COMMENT '报价的小数点后位数(所有价格都以整数存储)',
  `fee` float DEFAULT NULL COMMENT '手续费(默认)',
  `fee_type` enum('Amount','Slot') DEFAULT NULL,
  `margin` int(11) DEFAULT NULL COMMENT '保证金率(默认)',
  PRIMARY KEY (`code`)
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

-- Dump completed on 2013-08-22 22:44:00
