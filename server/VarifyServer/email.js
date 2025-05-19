// 发送邮件库
const nodemailer = require('nodemailer');
// 导入config.js
const config_module = require("./config")

/**
 * 创建发送邮件的代理
 */
let transport = nodemailer.createTransport({
    host: 'smtp.163.com', // 服务地址
    port: 465, //服务端口
    secure: true, // 是否有安全验证
    auth: {
        user: config_module.email_user, // 发送方邮箱地址
        pass: config_module.email_pass // 邮箱授权码或者密码SMTP
    }
});

/**
 * 发送邮件的函数
 * @param {*} mailOptions_ 发送邮件的参数
 * @returns 
 */
function SendMail(mailOptions_) {
    // 将异步转成同步的,阻塞等待 正常resolve 异常reject
    return new Promise(function (resolve, reject) {
        // 邮件的发送完成通过回调来通知, 是一个异步处理的函数
        transport.sendMail(mailOptions_, function (error, info) {
            if (error) {
                console.log(error);
                reject(error);
            } else {
                console.log('邮件已成功发送：' + info.response);
                resolve(info.response)
            }
        });
    })

}

module.exports.SendMail = SendMail