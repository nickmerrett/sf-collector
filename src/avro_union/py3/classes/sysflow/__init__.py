

from .schema_classes import SchemaClasses, SCHEMA as my_schema, get_schema_type
from avro.io import DatumReader


class SpecificDatumReader(DatumReader):
    SCHEMA_TYPES = {
        "sysflow.SysFlow": SchemaClasses.sysflow.SysFlowClass,
        "sysflow.entity.Process": SchemaClasses.sysflow.entity.ProcessClass,
        "sysflow.flow.ProcessFlow": SchemaClasses.sysflow.flow.ProcessFlowClass,
        "sysflow.type.ActionType": SchemaClasses.sysflow.type.ActionTypeClass,
        "sysflow.type.EventType": SchemaClasses.sysflow.type.EventTypeClass,
    }
    def __init__(self, readers_schema=None, **kwargs):
        writers_schema = kwargs.pop("writers_schema", readers_schema)
        writers_schema = kwargs.pop("writer_schema", writers_schema)
        super(SpecificDatumReader, self).__init__(writers_schema, readers_schema, **kwargs)
    def read_record(self, writers_schema, readers_schema, decoder):
        
        result = super(SpecificDatumReader, self).read_record(writers_schema, readers_schema, decoder)
        
        if readers_schema.fullname in SpecificDatumReader.SCHEMA_TYPES:
            result = SpecificDatumReader.SCHEMA_TYPES[readers_schema.fullname](result)
        
        return result