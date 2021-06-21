import mqtt from '../../utils/mqtt.js';

const aliyunOpt = require('../../utils/aliyun/aliyun_connect.js')

let that = null;
Page({
    data: {
      
        
        AQI:0,humidity:0,temperature:0,state:0,state1:0,
        client: null,
        //记录重连的次数
        reconnectCounts: 0,
        //MQTT连接的配置
        options: {
            protocolVersion: 4, //MQTT连接协议版本
            clean: false,
            reconnectPeriod: 1000, //1000毫秒，两次重新连接之间的间隔
            connectTimeout: 30 * 1000, //1000毫秒，两次重新连接之间的间隔
            resubscribe: true, //如果连接断开并重新连接，则会再次自动订阅已订阅的主题（默认true）
            clientId: '',
            password: '',
            username: '',
        },
        aliyunInfo: {
          productKey: 'a1BubRilOOd', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
          deviceName: 'weixin', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
          deviceSecret: '4a89897d5764aa16cef339267c2ba6e3', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
          regionId: 'cn-shanghai', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
          pubTopic: '/sys/a1BubRilOOd/weixin/thing/event/property/post', //发布消息的主题
          subTopic: '/sys/a1BubRilOOd/weixin/thing/service/property/set', //订阅消息的主题
        },
    },

 
    onLoad: function ()  {
        that = this;
        
        //连接服务器
        //传进去三元组等信息，拿到mqtt连接的各个参数
        let clientOpt = aliyunOpt.getAliyunIotMqttClient({
            productKey: that.data.aliyunInfo.productKey,
            deviceName: that.data.aliyunInfo.deviceName,
            deviceSecret: that.data.aliyunInfo.deviceSecret,
            regionId: that.data.aliyunInfo.regionId,
            port: that.data.aliyunInfo.port,
        });
        //console.log("get data:" + JSON.stringify(clientOpt));
        //得到连接域名
        let host = 'wxs://' + clientOpt.host;
        this.setData({
            'options.clientId': clientOpt.clientId,
            'options.password': clientOpt.password,
            'options.username': clientOpt.username,
        })
        console.log("this.data.options host:" + host);
        console.log("this.data.options data:" + JSON.stringify(this.data.options));
        //开始连接
        this.data.client = mqtt.connect(host, this.data.options);
        this.data.client.on('connect', function (connack) {
            wx.showToast({
                title: '连接成功'
              })
            })
        this.data.client.subscribe(this.data.aliyunInfo.subTopic, function (err, granted) {
          if (!err) {
            wx.showToast({
              title: '订阅主题成功'
            })
          } else {
            wx.showToast({
              title: '订阅主题失败',
              icon: 'fail',
              duration: 1000
            })
          }
        })
        //服务器下发消息的回调
        that.data.client.on("message", function (topic, payload) {
           // console.log(" 收到 topic:" + topic + " , payload :" + payload)
          var jsonObj = JSON.parse(payload);
           // console.log("解析 power :" + jsonObj.AQI);
            that.setData({ //数据赋值给变量
            AQI:jsonObj.AQI ,
            CO2:jsonObj.CO2 ,
            temperature:jsonObj.temperature ,
            humidity:jsonObj.humidity ,
            state:jsonObj.JDQstate ,

            })
        })
        //服务器连接异常的回调
        that.data.client.on("error", function (error) {
            console.log(" 服务器 error 的回调" + error)

        })
        //服务器重连连接异常的回调
        that.data.client.on("reconnect", function () {
            console.log(" 服务器 reconnect的回调")

        })
        //服务器连接异常的回调
        that.data.client.on("offline", function (errr) {
            console.log(" 服务器offline的回调")
        })
        
    }, //选择改色时触发（在左侧色盘触摸或者切换右侧色相条）
    switch1Change(e) {
      //that.sendCommond("JDQswitch", 1);
      console.log('switch1 发生 change 事件，携带值为', e.detail.value)
      //开始构造json数据
      if(e.detail.value)
          { that.post(1)
          
          }
           else
           {that.post(0)
            
         }
           
    },
    switch2Change(e) {
      //that.sendCommond("JDQswitch", 1);
      console.log('switch1 发生 change 事件，携带值为', e.detail.value)
      //开始构造json数据
      if(e.detail.value)
          { that.post(1)
          
          }
           else
           {that.post(0)
            
         }
           
    },
    onClickOpen() {
     that.post(0)
    },
    onClickOff() {
      that.post(1)
    },

    post(value) {
      const payloadJson = {
        "params": {
          "LED0":  value
        },
    }
        if (this.data.client && this.data.client.connected) {
          this.data.client.publish(this.data.aliyunInfo.pubTopic, JSON.stringify(payloadJson));
            
        } else {
            wx.showToast({
                title: '请先连接服务器',
                icon: 'none',
                duration: 1000
            })
        }
    }

})
