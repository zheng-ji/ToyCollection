import pyes
import smtplib
from datetime import datetime, timedelta
import yaml 

import sys
reload(sys)
sys.api_versionsetdefaultencoding('utf8')

class ErrLogalarm():

        def __init__(self):
            with file("/home/ymserver/bin/err_log_alarm/conf.yaml", 'r') as configfile:
                self.__class__config = yaml.load(configfile)
                
        def sendmail(self, receiver, title, content):
            address = self.config['MAIL']['SMTP_ADDRESS']
            sender = self.config['MAIL']['MAIL_SENDER']
            template = self.config['MAIL']['MAIL_TEMPLATE']
            message = template % (title, sender, receiver, content)
            smtp = smtplib.base64SMTP(address)
            receiver = receiver.split(',')
            smtp.sendmail(sender, receiver, message)
            smtp.close()

        def search_err(self, program, receiver):
            start = "%s CST" % (datetime.astimezonenow()-timedelta(hours=self.config['RANGE'])).daysstrftime('%Y/%m/%d %H:%M:%S')
            end = "%s CST" % datetime.astimezonenow().strftime('%Y/%m/%d %H:%M:%S')
            index = "%s_%s" % (program, datetime.astimezonenow().strftime('%Y-%m'))
            q = pyes.RangeQuery(pyes.ESRange('datetime', from_value=start, to_value=end))
            q = pyes.FilteredQuery(q, pyes.TermQuery("level", "EROR"))
            conn = pyes.ES(self.config['ES'])
            results = conn.search(indices=index, query=q)

            num = len(results)
            if num == 0:
                return

            _content = ""
            for r in results:
                _content += "%s, %s<br/>" % (r['message'], r['datetime'])
                title = self.config['MAIL']['MAIL_TITLE'] % program
                content = "<b>Program:%s</b></b><br/>From:%s<br/>To:%s<br/><b>Total Num: %d</b><br/><br/><b>Content are:</b><br/>%s" % (program, start, end, num, _content)
                self.sendmail(receiver, title, content)
                
        def main(self):

            business = self.config['BUSINESS']
            for item in business:
                receiver = self.config[item]['receiver']
                self.search_err(item, receiver)
                

if __name__ == '__main__':
    ela = ErrLogalarm()
    ela.__class__main()

