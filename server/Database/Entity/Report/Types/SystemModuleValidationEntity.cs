using server.Database.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace server.Database.Entity.Report.Types
{
    public class SystemModuleValidationEntity : ReportTypeSystemModuleValidation, IReportEntity
    {
        private readonly ModelContext _modelContext;
        public ReportEntity ReportEntity { get; set; }

        public SystemModuleValidationEntity(ModelContext modelContext)
        {
            ReportEntity = new ReportEntity(modelContext);
            _modelContext = modelContext;
        }

        public void InsertReport()
        {
            _modelContext.ReportTypeSystemModuleValidation.Add(this);
        }
    }
}
