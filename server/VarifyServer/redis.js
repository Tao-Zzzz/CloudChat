const config_module = require('./config')
const Redis = require('ioredis')

// 创建客户端实例
const RedisCli = new Redis({
    host : config_module.redis_host,
    port : config_module.redis_port,
    password: config_module.redis_passwd,
});

// 监听错误信息, 连接失败了就会调用回调函数
RedisCli.on("error", function(err){
    console.log("RedisCli connect error");
    // 直接退出
    RedisCli.quit();
});


// 封装get
async function GetRedis(key) {
    try {
        // await等待一个promise操作，就相当于阻塞，要用async要async异步声明
        const result = await RedisCli.get(key)
        if (result === null) {
            console.log('result:', '<' + result + '>', 'This key cannot be find...')
            return null
        }
        console.log('Result:', '<' + result + '>', 'Get key success!...');
        return result
    } catch (error) {
        console.log('GetRedis error is', error);
        return null
    }
}

// 封装exists
async function QueryRedis(key) {
    try {
        const result = await RedisCli.exists(key)
        //  判断该值是否为空 如果为空返回null
        if (result === 0) {
            console.log('result:<', '<' + result + '>', 'This key is null...');
            return null
        }
        console.log('Result:', '<' + result + '>', 'With this value!...');
        return result
    } catch (error) {
        console.log('QueryRedis error is', error);
        return null
    }
}

// 封装set，并且加上过期时间
async function SetRedisExpire(key, value, exptime) {
    try {
        // 设置键和值
        await RedisCli.set(key, value)
        // 设置过期时间（以秒为单位）
        await RedisCli.expire(key, exptime);
        return true;
    } catch (error) {
        console.log('SetRedisExpire error is', error);
        return false;
    }
}

// 退出，相当于释放内存
function Quit() {
    RedisCli.quit();
}

// 抛出一些操作接口给其他js使用
module.exports = { GetRedis, QueryRedis, Quit, SetRedisExpire, }