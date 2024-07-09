#include "txbench/hybench/Generator.hpp"
#include <chrono>
#include <iostream>
#include <unordered_map>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::hybench {
//---------------------------------------------------------------------------
Generator::Generator(Config& config, uint64_t seed)
   : Random(seed), config(config)
// Constructor
{
   static const vector<pair<string, string>> rawCities = {{"Guangdong", "Shenzhen"}, {"Guangdong", "Guangzhou"}, {"Guangdong", "Foshan"}, {"Guangdong", "Dongguan"}, {"Guangdong", "Huizhou"}, {"Guangdong", "Zhuhai"}, {"Guangdong", "Maoming"}, {"Guangdong", "Jiangmen"}, {"Guangdong", "Zhongshan"}, {"Guangdong", "Zhanjiang"}, {"Guangdong", "Shantou"}, {"Guangdong", "Zhaoqing"}, {"Guangdong", "Jieyang"}, {"Guangdong", "Qingyuan"}, {"Guangdong", "Shaoguan"}, {"Guangdong", "Yangjiang"}, {"Guangdong", "Meizhou"}, {"Guangdong", "Shanwei"}, {"Guangdong", "Heyuan"}, {"Guangdong", "Chaozhou"}, {"Guangdong", "Yunfu"}, {"Jiangsu", "Suzhou"}, {"Jiangsu", "Nanjing"}, {"Jiangsu", "Wuxi"}, {"Jiangsu", "Nantong"}, {"Jiangsu", "Changzhou"}, {"Jiangsu", "Xuzhou"}, {"Jiangsu", "Yangzhou"}, {"Jiangsu", "Yancheng"}, {"Jiangsu", "Taizhou"}, {"Jiangsu", "Zhenjiang"}, {"Jiangsu", "Huaian"}, {"Jiangsu", "Lianyungang"}, {"Jiangsu", "Suqian"}, {"Shandong", "Qingdao"}, {"Shandong", "Jinan"}, {"Shandong", "Yantai"}, {"Shandong", "Weifang"}, {"Shandong", "Linyi"}, {"Shandong", "Jining"}, {"Shandong", "Zibo"}, {"Shandong", "Heze"}, {"Shandong", "Dezhou"}, {"Shandong", "Weihai"}, {"Shandong", "Dongying"}, {"Shandong", "Taian"}, {"Shandong", "Binzhou"}, {"Shandong", "Liaocheng"}, {"Shandong", "Rizhao"}, {"Shandong", "Zaozhuang"}, {"Zhejiang", "Hangzhou"}, {"Zhejiang", "Ningbo"}, {"Zhejiang", "Wenzhou"}, {"Zhejiang", "Shaoxing"}, {"Zhejiang", "Jiaxing"}, {"Zhejiang", "Taizhou"}, {"Zhejiang", "Jinhua"}, {"Zhejiang", "Huzhou"}, {"Zhejiang", "Quzhou"}, {"Zhejiang", "Lishui"}, {"Zhejiang", "Zhoushan"}, {"Henan", "Zhengzhou"}, {"Henan", "Luoyang"}, {"Henan", "Nanyang"}, {"Henan", "Xuchang"}, {"Henan", "Zhoukou"}, {"Henan", "Xinxiang"}, {"Henan", "Shangqiu"}, {"Henan", "Zhumadian"}, {"Henan", "Xinyang"}, {"Henan", "Pingdingshan"}, {"Henan", "Kaifeng"}, {"Henan", "Anyang"}, {"Henan", "Jiaozuo"}, {"Henan", "Puyang"}, {"Henan", "Luohe"}, {"Henan", "Sanmenxia"}, {"Henan", "Hebi"}, {"Henan", "Jiyuan"}, {"Sichuan", "Chengdu"}, {"Sichuan", "Mianyang"}, {"Sichuan", "Yibin"}, {"Sichuan", "Deyang"}, {"Sichuan", "Nanchong"}, {"Sichuan", "Luzhou"}, {"Sichuan", "Dazhou"}, {"Sichuan", "Leshan"}, {"Sichuan", "Liangshan"}, {"Sichuan", "Neijiang"}, {"Sichuan", "Zigong"}, {"Sichuan", "Meishan"}, {"Sichuan", "Suining"}, {"Sichuan", "Guangan"}, {"Sichuan", "Panzhihuang"}, {"Sichuan", "Guangyuan"}, {"Sichuan", "Ziyang"}, {"Sichuan", "Yaan"}, {"Sichuan", "Bazhong"}, {"Sichuan", "Aba"}, {"Sichuan", "Ganzi"}, {"Hubei", "Wuhan"}, {"Hubei", "Xiangyang"}, {"Hubei", "Yichang"}, {"Hubei", "Jingzhou"}, {"Hubei", "Xianggan"}, {"Hubei", "Huanggang"}, {"Hubei", "Shiyan"}, {"Hubei", "Jingmen"}, {"Hubei", "Huangshi"}, {"Hubei", "Xianning"}, {"Hubei", "Suizhou"}, {"Hubei", "Enshi"}, {"Hubei", "EZhou"}, {"Fujian", "Fuzhou"}, {"Fujian", "Quanzhou"}, {"Fujian", "Xiamen"}, {"Fujian", "Zhangzhou"}, {"Fujian", "Ningde"}, {"Fujian", "Longyan"}, {"Fujian", "Sanming"}, {"Fujian", "Putian"}, {"Fujian", "Nanping"}, {"Hunan", "Changsha"}, {"Hunan", "Yueyang"}, {"Hunan", "Changde"}, {"Hunan", "Hengyang"}, {"Hunan", "Zhuzhou"}, {"Hunan", "Chenzhou"}, {"Hunan", "Xiangtan"}, {"Hunan", "Shaoyang"}, {"Hunan", "Yongzhou"}, {"Hunan", "Yiyang"}, {"Hunan", "Loudi"}, {"Hunan", "Huaihua"}, {"Hunan", "Xiangxi"}, {"Hunan", "Zhangjiajie"}, {"Shanghai", "Shanghai"}, {"Anhui", "Hefei"}, {"Anhui", "Wuhu"}, {"Anhui", "Chuzhou"}, {"Anhui", "Fuyang"}, {"Anhui", "Anqing"}, {"Anhui", "Maanshan"}, {"Anhui", "Suzhou"}, {"Anhui", "Bengbu"}, {"Anhui", "Bozhou"}, {"Anhui", "Luan"}, {"Anhui", "Xuancheng"}, {"Anhui", "Huainan"}, {"Anhui", "Huaibei"}, {"Anhui", "Tongling"}, {"Anhui", "Chizhou"}, {"Anhui", "Huangshan"}, {"Hebei", "Tangshan"}, {"Hebei", "Shijiazhuang"}, {"Hebei", "Cangzhou"}, {"Hebei", "Handan"}, {"Hebei", "Baoding"}, {"Hebei", "Langfang"}, {"Hebei", "Xingtai"}, {"Hebei", "Qinhuangdao"}, {"Hebei", "Hengshui"}, {"Hebei", "Zhangjiakou"}, {"Hebei", "Chengde"}, {"Beijing", "Beijing"}, {"Shaanxi", "Taiyuan"}, {"Shaanxi", "Changzhi"}, {"Shaanxi", "Lvliang"}, {"Shaanxi", "Jincheng"}, {"Shaanxi", "Yuncheng"}, {"Shaanxi", "Linfen"}, {"Shaanxi", "Jinzhong"}, {"Shaanxi", "Datong"}, {"Shaanxi", "Shuozhou"}, {"Shaanxi", "Xinzhou"}, {"Shaanxi", "Yangquan"}, {"Jiangxi", "Nanchang"}, {"Jiangxi", "Jindezhen"}, {"Jiangxi", "Pingxiang"}, {"Jiangxi", "Jiujiang"}, {"Jiangxi", "Xinyu"}, {"Jiangxi", "Yingtan"}, {"Jiangxi", "Ganzhou"}, {"Jiangxi", "Jian"}, {"Jiangxi", "Yichun"}, {"Jiangxi", "Fuzhou"}, {"Jiangxi", "Shangrao"}, {"Chongqing", "Chongqing"}, {"Liaoning", "Dalian"}, {"Liaoning", "Shenyang"}, {"Liaoning", "Anshan"}, {"Liaoning", "Yingkou"}, {"Liaoning", "Panjin"}, {"Liaoning", "Jinzhou"}, {"Liaoning", "Chaoyang"}, {"Liaoning", "Benxi"}, {"Liaoning", "Fushun"}, {"Liaoning", "Liaoyang"}, {"Liaoning", "Dandong"}, {"Liaoning", "Huludao"}, {"Liaoning", "Tieling"}, {"Liaoning", "Fuxin"}, {"Yunnan", "Kunming"}, {"Yunnan", "Qujing"}, {"Yunnan", "Honghe"}, {"Yunnan", "Yuxi"}, {"Yunnan", "Chuxiong"}, {"Yunnan", "Dali"}, {"Yunnan", "Zhaotong"}, {"Yunnan", "Wenshan"}, {"Yunnan", "Baoshan"}, {"Yunnan", "Puer"}, {"Yunnan", "Lincang"}, {"Yunnan", "Xishuangbannan"}, {"Yunnan", "Dehong"}, {"Yunnan", "Lijiang"}, {"Yunnan", "Diqing"}, {"Yunnan", "Nujiang"}, {"Guangxi", "Nanning"}, {"Guangxi", "Liuzhou"}, {"Guangxi", "Guilin"}, {"Guangxi", "Yulin"}, {"Guangxi", "Qinzhou"}, {"Guangxi", "Baise"}, {"Guangxi", "Guigang"}, {"Guangxi", "Beihai"}, {"Guangxi", "Wuzhou"}, {"Guangxi", "Hechi"}, {"Guangxi", "Chongzuo"}, {"Guangxi", "Fangchenggang"}, {"Guangxi", "Laibin"}, {"Hong_Kong", "Hong_Kong"}, {"Shanxi", "Xian"}, {"Shanxi", "Yulin"}, {"Shanxi", "Xianyang"}, {"Shanxi", "Baoji"}, {"Shanxi", "Yanan"}, {"Shanxi", "Weinan"}, {"Shanxi", "Hanzhong"}, {"Shanxi", "Ankang"}, {"Shanxi", "Shangluo"}, {"Shanxi", "Tongchuang"}, {"Inner_Mongolia", "Ordos"}, {"Inner_Mongolia", "Baotou"}, {"Inner_Mongolia", "Hohhot"}, {"Inner_Mongolia", "Chifeng"}, {"Inner_Mongolia", "Tongliao"}, {"Inner_Mongolia", "Hulunbuir"}, {"Inner_Mongolia", "Xilinhot"}, {"Inner_Mongolia", "Ulanqab"}, {"Inner_Mongolia", "Wuhai"}, {"Inner_Mongolia", "Qingγan"}, {"Inner_Mongolia", "Alshaa"}, {"Guizhou", "Guiyang"}, {"Guizhou", "Zunyi"}, {"Guizhou", "Bijie"}, {"Guizhou", "Qiannan"}, {"Guizhou", "Liupanshui"}, {"Guizhou", "Tongren"}, {"Guizhou", "Qianxi"}, {"Guizhou", "Qiandongnan"}, {"Guizhou", "Anshun"}, {"Xinjiang", "Urumqi"}, {"Xinjiang", "Changji"}, {"Xinjiang", "Aksu"}, {"Xinjiang", "Yili"}, {"Xinjiang", "Kashgar"}, {"Xinjiang", "Bayin"}, {"Xinjiang", "Karamay"}, {"Xinjiang", "Tacheng"}, {"Xinjiang", "Hami"}, {"Xinjiang", "Hotan"}, {"Xinjiang", "Bortala"}, {"Xinjiang", "Turpan"}, {"Xinjiang", "Altay"}, {"Xinjiang", "Kizilsu"}, {"Tianjin", "Tianjin"}, {"Heilongjiang", "Harbin"}, {"Heilongjiang", "Daqing"}, {"Heilongjiang", "Qiqihar"}, {"Heilongjiang", "Suihua"}, {"Heilongjiang", "Mudanjiang"}, {"Heilongjiang", "Jiamusi"}, {"Heilongjiang", "Jixi"}, {"Heilongjiang", "Shuangyashan"}, {"Heilongjiang", "Heihe"}, {"Heilongjiang", "Hegang"}, {"Heilongjiang", "Qitaihe"}, {"Heilongjiang", "Yichun"}, {"Heilongjiang", "Daxinganling"}, {"Jilin", "Changchun"}, {"Jilin", "Jilin"}, {"Jilin", "Songyuan"}, {"Jilin", "Yanbian"}, {"Jilin", "Tonghua"}, {"Jilin", "Siping"}, {"Jilin", "Baicheng"}, {"Jilin", "Baishan"}, {"Jilin", "Liaoyuan"}, {"Jilin", "Meihekou"}, {"Jilin", "Changbaishan"}, {"Gansu", "Lanzhou"}, {"Gansu", "Qingyang"}, {"Gansu", "Jiuquan"}, {"Gansu", "Tianshui"}, {"Gansu", "Baiyin"}, {"Gansu", "Wuwei"}, {"Gansu", "Pingliang"}, {"Gansu", "Longnan"}, {"Gansu", "Jinchang"}, {"Gansu", "Zhangye"}, {"Gansu", "Dingxi"}, {"Gansu", "Linxia"}, {"Gansu", "Jiayuguan"}, {"Gansu", "Gannan"}, {"Hainan", "Haikou"}, {"Hainan", "Sanya"}, {"Hainan", "Danzhou"}, {"Hainan", "Tsingmai"}, {"Hainan", "Qionghai"}, {"Hainan", "Wenchang"}, {"Hainan", "Wanning"}, {"Hainan", "Linshui"}, {"Hainan", "Lingao"}, {"Hainan", "Dongfang"}, {"Hainan", "Ledong"}, {"Hainan", "Changjiang"}, {"Hainan", "Dingan"}, {"Hainan", "Tunchang"}, {"Hainan", "Qiongzhong"}, {"Hainan", "Baoting"}, {"Hainan", "Baisha"}, {"Hainan", "Wuzhishan"}, {"Ningxia", "Yinchuan"}, {"Ningxia", "Wuzhong"}, {"Ningxia", "Shizuishan"}, {"Ningxia", "Zhongwei"}, {"Ningxia", "Guyuan"}, {"Qinghai", "Xining"}, {"Qinghai", "Haixi"}, {"Qinghai", "Haidong"}, {"Qinghai", "Hainan"}, {"Qinghai", "Huangnan"}, {"Qinghai", "Haibei"}, {"Qinghai", "Guoluo"}, {"Qinghai", "Yushu"}, {"Tibet", "Lhasa"}, {"Tibet", "Xigaze"}, {"Tibet", "Qamdo"}, {"Tibet", "Shannan"}, {"Tibet", "Nyingchi"}, {"Tibet", "Nagqu"}, {"Tibet", "Ali"}, {"Macau", "Macau"}, {"Taiwan", "Taiwan"}};
   for (const auto& city : rawCities) {
      provinces.push_back(city.first);
      cities[city.first].push_back(city.second);
   }
}
//---------------------------------------------------------------------------
std::string Generator::generateGender()
// Generate a random gender
{
   return generateString({"female", "male"});
}
//---------------------------------------------------------------------------
std::string Generator::generateName(const std::string& gender)
// Generate a random name
{
   static const vector<string> firstNamesMale = {"Li", "Wei", "Bin", "Bo", "Hui", "Feng", "Long", "Qiang", "Lei", "Ling", "Jun", "Yang", "Yong", "Jie", "Tao", "Ming", "Chao", "Chen", "Ping", "Peng", "Gang", "Xiang", "Shuai", "Muchen", "Haoyu", "Haowen", "Haoran", "Wenbo", "Bowen", "Yihan", "Mingze", "Yichen", "Yuze", "Yuxing", "Xinning", "Haoran", "Yize", "Yuxuan", "Muyang", "Jianguo", "Jianhua", "Jianyong", "Guohua", "Guoliang", "Heping", "Jianping", "Jiaheng", "Pengfei", "Zhiming", "Deming", "Yuhang", "Dehua", "Junjie"};
   static const vector<string> firstNameFemale = {"Li", "Wei", "Dan", "Hui", "Fang", "Feng", "Wen", "Na", "Min", "Jing", "Lei", "Ling", "Yang", "Yan", "Jie", "Juan", "Tao", "Ming", "Hong", "Chao", "Chen", "Ping", "Ting", "Mei", "Xue", "Yue", "Qian", "Ying", "Ruoxi", "Yinuo", "Yihan", "Zihan", "Yimo", "Yutong", "Xinyi", "Yutong", "Yuhan", "Yuxi", "Chenyang", "Xinyang", "Xiuying", "Guiying", "Xiulan", "Yulan", "Guilan", "Xiuzhen", "Fengying", "Yuzhen", "Yuying", "Lanying"};
   static const vector<string> lastNames = {"Wang", "Li", "Zhang", "Liu", "Chen", "Yang", "Huang", "Zhao", "Wu", "Zhou", "Xu", "Sun", "Ma", "Zhu", "Hu", "Guo", "He", "Lin", "Luo", "Gao", "Zheng", "Liang", "Xie", "Song", "Tang", "Xu", "Han", "Deng", "Feng", "Cao", "Peng", "Zeng", "Xiao", "Tian", "Dong", "Pan", "Yuan", "Cai", "Jiang", "Yu", "Yu", "Du", "Ye", "Cheng", "Wei", "Su", "Lu", "Ding", "Ren", "Lu", "Yao", "Shen", "Zhong", "Jiang", "Cui", "Tan", "Lu", "Fan", "Wang", "Liao", "Shi", "Jin", "Wei", "Jia", "Xia", "Fu", "Fang", "Zou", "Xiong", "Bai", "Meng", "Qin", "Qiu", "Hou", "Jiang", "Yin", "Xue", "Yan", "Duan", "Lei", "Long", "Li", "Shi", "Tao", "He", "Mao", "Hao", "Gu", "Gong", "Shao", "Wan", "Qin", "Wu", "Qian", "Dai", "Yan", "Mo", "Kong", "Xiang", "Chang"};

   return generateString(lastNames) + " " + generateString(gender == "male" ? firstNamesMale : firstNameFemale);
}
//---------------------------------------------------------------------------
uint64_t Generator::generateCustomerId()
// Generate a random customer id
{
   return generateRand(1ul, config.customerNumber);
}
//---------------------------------------------------------------------------
uint64_t Generator::generateCompanyId()
// Generate a random company id
{
   return generateRand(config.customerNumber + 1, config.customerNumber + config.companyNumber);
}
//---------------------------------------------------------------------------
uint64_t Generator::generateCustomerOrCompanyId()
// Generate a random customer or company id
{
   return generateRand(1ul, config.customerNumber + config.companyNumber);
}
//---------------------------------------------------------------------------
int Generator::generateAge()
// Generate a random age
{
   return generateRand(config.customerAgeLower, config.customerAgeUpper);
}
//---------------------------------------------------------------------------
std::string Generator::generatePhone()
// Generate a random phone number
{
   static const vector<string> prefix = {"134", "135", "136", "137", "138", "139", "150", "151", "152", "157", "158", "159", "182", "183", "184", "187", "188", "178", "147", "172", "198", "130", "131", "132", "145", "155", "156", "166", "171", "175", "176", "185", "186", "166", "133", "149", "153", "173", "177", "180", "181", "189", "199"};
   auto phone = generateString(prefix);
   //last_8 digit
   for (int i = 0; i < 8; i++) {
      phone += to_string(generateRand(0, 9));
   }
   return phone;
}
//---------------------------------------------------------------------------
std::string Generator::generateProvince()
// Generate a random province
{
   return generateString(provinces);
}
//---------------------------------------------------------------------------
std::string Generator::generateCity(const std::string& province)
// Generate a random city
{
   return generateString(cities[province]);
}
//---------------------------------------------------------------------------
Generator::date Generator::generateDateBeforeMidPoint()
// Generate a random date
{
   auto start = std::chrono::system_clock::from_time_t(std::mktime(&config.startDate));
   auto end = std::chrono::system_clock::from_time_t(std::mktime(&config.midPointDate));
   auto startMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

   return start + std::chrono::milliseconds(generateRand(0l, startMs));
}
//---------------------------------------------------------------------------
Generator::date Generator::generateDateAfterMidPoint()
// Generate a random date
{
   auto start = std::chrono::system_clock::from_time_t(std::mktime(&config.midPointDate));
   auto end = std::chrono::system_clock::from_time_t(std::mktime(&config.endDate));
   auto startMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

   return start + std::chrono::milliseconds(generateRand(0l, startMs));
}
//---------------------------------------------------------------------------
std::string Generator::generateCompanyCategory()
// Generate a random company category
{
   static const vector<string> companyCategories = {"agriculture", "forestry", "animal_husbandry", "fishery", "agriculture_auxiliary", "coal_mining", "oil_gas_mining", "black_medal_mining", "nonferrous_metal_mining", "nonmetal_mining", "mining_auxiliary", "other_mining", "agriculture_food_processing", "food_manufacturing", "alcohol_drink_tea", "tobacco_manufacturing", "textile_industry", "textile_garment", "leather_clothing", "wood_bamboo_processing", "furniture_manufacturing", "paper_industry", "printing_business", "office_art_sports_entertainment_supplies", "oil_coal_processing", "chemistry_manufacturing", "medical_care_medicines", "chemical_fiber_manufacturing", "rubber_plastic", "nonmetal_manufacturing", "black_medal_manufacturing", "nonferrous_metal_manufacturing", "medal_manufacturing", "general_equipment_manufacturing", "special_equipment_manufacturing", "automotive_manufacturing", "railway_ship_aviation_manufacturing", "electrical_machinery_manufacturing", "computer_communication_manufacturing", "instrument_manufacturing", "other_manufacturing", "abandoned_resource_utilization", "medal_machinery_equipment_repair", "electricity_manufacturing_supply", "gas_manufacturing_supply", "water_manufacturing_supply", "house_construction", "civil_engineering", "building_installation", "building_decoration", "wholesale", "retail", "railway_transportation", "road_transportation", "water_transportation", "pipeline_transportation", "multi_transportation_agent", "loading_warehousing", "postal_service", "accommodation", "catering", "telecommunication", "internet_service", "software_IT", "money_finance", "capital_market", "insurance", "other_finance", "real_estate", "leasing_service", "business_service", "research", "technology_service", "promotion_service", "water_management", "eco_environmental_management", "public_facility_management", "land_management", "resident_service", "vehicle_electronic_product_repair", "other_service", "education", "health", "social_work", "news_publishing", "broadcasting_recoding_filming", "culture_art", "sports", "entertainment", "public_administration", "national_institution", "consultative_institution", "social_protection", "social_group", "the_masses", "international_institution"};
   return generateString(companyCategories);
}
//---------------------------------------------------------------------------
int Generator::generateCompanyStaff()
// Generate a random company staff
{
   return generateRand(config.companySizeLower, config.companySizeUpper);
}
//---------------------------------------------------------------------------
std::string Generator::generateCustomerTransferType()
// Generate a random customer transfer type
{
   return generateString({"transfer", "red_packet", "donate", "invest"});
}
//---------------------------------------------------------------------------
std::string Generator::generateCompanyTransferType()
// Generate a random company transfer type
{
   return generateString({"business", "salary", "service", "invest"});
}
//---------------------------------------------------------------------------
std::string Generator::generateCustomerCheckingType()
// Generate a random customer checking type
{
   return generateString({"check", "others"});
}
//---------------------------------------------------------------------------
std::string Generator::generateCompanyCheckingType()
// Generate a random company checking type
{
   return generateString({"business", "service", "invest"});
}
//---------------------------------------------------------------------------
Generator::date Generator::generateLoanDate()
// Generate a random loan date
{
   auto start = std::chrono::system_clock::from_time_t(std::mktime(&config.loanDate));
   auto end = std::chrono::system_clock::from_time_t(std::mktime(&config.endDate));
   auto startMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

   return start + std::chrono::milliseconds(generateRand(0l, startMs));
}
//---------------------------------------------------------------------------
int Generator::generateLoanDuration()
// Generate a random loan duration
{
   vector<int> durations = {30, 60, 90, 180, 365};
   return durations[generateRand(0ul, durations.size() - 1)];
}
//---------------------------------------------------------------------------
std::string Generator::generateLoanStatus()
// Generate a random loan status
{
   return generateString({"under_review", "reject", "accept"});
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
