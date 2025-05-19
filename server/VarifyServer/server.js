const grpc = require('@grpc/grpc-js')
const message_proto = require('./proto')
const const_module = require('./const')
const {v4 : uuidv4} = require('uuid')
const emailModule = require('./email')
const redis_module = require('./redis')

// 请求call 和回复 callback, 对应grpc的service
// async声明异步函数
async function GetVarifyCode(call, callback) {
    console.log("email is ", call.request.email)
    // 因为SendMail有reject,返回的就是异常,所以要用try/catch捕获
    try {
        // 生成验证码,先查找是否已经生成过, 前缀加上邮箱地址作为一个key
        let redis_key = const_module.code_prefix + call.request.email
        let query_res = await redis_module.GetRedis(redis_key)
        console.log("query_res is ", query_res)
        let uniqueId = query_res;
        // 如果没有生成过
        if(query_res == null){
            uniqueId = uuidv4();
            if(uniqueId.length > 4){
                uniqueId = uniqueId.substring(0, 4);
            }
            let bres = await redis_module.SetRedisExpire(redis_key, uniqueId, 600)
            if(!bres){
                // grpc返回设置失败的错误
                callback(null, {email: call.request.email,
                    error:const_module.Errors.RedisErr
                });
                return;
            }
        }
        console.log("uniqueId is ", uniqueId)
        let text_str = '您的验证码为' + uniqueId + '请三分钟内完成注册'
        //发送邮件
        let mailOptions = {
            from: 'z2194774308t@163.com',
            to: call.request.email, // 请求里携带的email
            subject: '验证码',
            text: text_str,
        }

        // await等待Promise完成,Promise有任意一个参数执行了就会结束
        // await 一定要在异步函数中使用,也就是async
        let send_res = await emailModule.SendMail(mailOptions);
        if (!send_res) {
            // grpc返回设置失败的错误
            callback(null, {
                email: call.request.email,
                error: const_module.Errors.RedisErr
            });
            return;
        }

        console.log("send res is ", send_res)

        // 参数的callback,也就是回复
        callback(null, {
            email: call.request.email,
            error: const_module.Errors.Success
        });


    } catch (error) {
        console.log("catch error is ", error)

        callback(null, {
            email: call.request.email,
            error: const_module.Errors.Exception
        });
    }

}

function main() {
    // 监听grpc客户端的消息
    var server = new grpc.Server()
    // GetVarifyCode既当key也当value, grpc的service加载
    server.addService(message_proto.VarifyService.service, { GetVarifyCode: GetVarifyCode })
    // 绑定好然后进行回调
    server.bindAsync('0.0.0.0:50051', grpc.ServerCredentials.createInsecure(), () => {
        // server.start()
        console.log('grpc server started')
    })
}

main()