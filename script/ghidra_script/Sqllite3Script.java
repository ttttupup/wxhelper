// sqlite function
//@author 
//@category _NEW_
//@keybinding 
//@menupath 
//@toolbar 

import java.util.regex.Pattern;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.lang.*;
import ghidra.util.exception.DuplicateNameException;
import ghidra.util.exception.InvalidInputException;
import ghidra.program.model.symbol.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;

public class Sqllite3Script extends GhidraScript {

	public void run() throws Exception {

		run2();

	}

	private void run2() {
		Listing listing = currentProgram.getListing();
		AddressSpace defaultAddressSpace = currentProgram.getAddressFactory().getDefaultAddressSpace();
		SymbolTable symbolTable = currentProgram.getSymbolTable();
		FunctionManager functionManager = currentProgram.getFunctionManager();
		Language language = currentProgram.getLanguage();
		LanguageDescription languageDescription = language.getLanguageDescription();
		int size = languageDescription.getSize() / 8;

		try {
			Address selectAddress = askAddress("选择地址", "选择一个sqlite3_api_routines结构体中的函数指针的地址");
			String xRefLabelName = getXRefLabelName(listing, functionManager, selectAddress);
			if (null != xRefLabelName) {
				Sqllite3Script.EnumSql byCode = EnumSql.getByCode(xRefLabelName);
				if (null != byCode) {
					Integer code = byCode.getCode();
					long offset = selectAddress.getOffset();
					long first = offset - code * size;
					for (EnumSql item : EnumSql.values()) {
						long addr = item.getCode() * size + first;
						setFuncName(listing, functionManager, defaultAddressSpace.getAddress(addr), symbolTable,
								item.getDesc());
						printf("sqlite3 function :{%s} \n", item.getDesc());

					}
				}
			}

		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	private String getXRefLabelName(Listing listing, FunctionManager functionManager, Address selectAddress) {
		String label = null;
		Data dataAt = listing.getDataAt(selectAddress);
		if (null == dataAt) {
			printerr("not found data from selectAddress");
			return label;
		}
		if (dataAt.isPointer()) {
			Reference[] referencesFrom = dataAt.getReferencesFrom();
			if (null == referencesFrom) {
				printerr("not found references");
				return label;
			}

			for (Reference reference : referencesFrom) {
				Address toAddress = reference.getToAddress();
				Function functionAt = functionManager.getFunctionAt(toAddress);

				if (null == functionAt) {
					CodeUnit codeUnitAt = listing.getCodeUnitAt(toAddress);
					if (null == codeUnitAt) {
						label = listing.getComment(CodeUnit.POST_COMMENT, toAddress);
						if (label == null) {
							label = listing.getComment(CodeUnit.PRE_COMMENT, toAddress);
						}
						if (label == null) {
							label = listing.getComment(CodeUnit.EOL_COMMENT, toAddress);
						}
					}
					label = codeUnitAt.getLabel();

				} else {
					label = functionAt.getName();
				}
				return label;
			}
		}
		return label;

	}

	private void setFuncName(Listing listing, FunctionManager functionManager, Address selectAddress,
			SymbolTable symbolTable, String name) throws DuplicateNameException, InvalidInputException {
		Data dataAt = listing.getDataAt(selectAddress);
		if (null == dataAt) {
			printerr("not found data from selectAddress");
			return;
		}
		if (dataAt.isPointer()) {
			Reference[] referencesFrom = dataAt.getReferencesFrom();
			if (null == referencesFrom) {
				printerr("not found references");
				return;
			}

			for (Reference reference : referencesFrom) {
				Address toAddress = reference.getToAddress();
				Function functionAt = functionManager.getFunctionAt(toAddress);

				if (null == functionAt) {
					CodeUnit codeUnitAt = listing.getCodeUnitAt(toAddress);
					if (null != codeUnitAt) {
						codeUnitAt.setComment(CodeUnit.POST_COMMENT, name);
					}
					symbolTable.createLabel(toAddress, name, SourceType.USER_DEFINED);
				} else {
					functionAt.setName(name, SourceType.USER_DEFINED);
				}
			}
		}

	}

	public enum EnumSql {

		SQLITE3_AGGREGATE_CONTEXT(0, "sqlite3_aggregate_context"),
		SQLITE3_AGGREGATE_COUNT(1, "sqlite3_aggregate_count"), SQLITE3_BIND_BLOB(2, "sqlite3_bind_blob"),
		SQLITE3_BIND_DOUBLE(3, "sqlite3_bind_double"), SQLITE3_BIND_INT(4, "sqlite3_bind_int"),
		SQLITE3_BIND_INT64(5, "sqlite3_bind_int64"), SQLITE3_BIND_NULL(6, "sqlite3_bind_null"),
		SQLITE3_BIND_PARAMETER_COUNT(7, "sqlite3_bind_parameter_count"),
		SQLITE3_BIND_PARAMETER_INDEX(8, "sqlite3_bind_parameter_index"),
		SQLITE3_BIND_PARAMETER_NAME(9, "sqlite3_bind_parameter_name"), SQLITE3_BIND_TEXT(10, "sqlite3_bind_text"),
		SQLITE3_BIND_TEXT16(11, "sqlite3_bind_text16"), SQLITE3_BIND_VALUE(12, "sqlite3_bind_value"),
		SQLITE3_BUSY_HANDLER(13, "sqlite3_busy_handler"), SQLITE3_BUSY_TIMEOUT(14, "sqlite3_busy_timeout"),
		SQLITE3_CHANGES(15, "sqlite3_changes"), SQLITE3_CLOSE(16, "sqlite3_close"),
		SQLITE3_COLLATION_NEEDED(17, "sqlite3_collation_needed"),
		SQLITE3_COLLATION_NEEDED16(18, "sqlite3_collation_needed16"), SQLITE3_COLUMN_BLOB(19, "sqlite3_column_blob"),
		SQLITE3_COLUMN_BYTES(20, "sqlite3_column_bytes"), SQLITE3_COLUMN_BYTES16(21, "sqlite3_column_bytes16"),
		SQLITE3_COLUMN_COUNT(22, "sqlite3_column_count"),
		SQLITE3_COLUMN_DATABASE_NAME(23, "sqlite3_column_database_name"),
		SQLITE3_COLUMN_DATABASE_NAME16(24, "sqlite3_column_database_name16"),
		SQLITE3_COLUMN_DECLTYPE(25, "sqlite3_column_decltype"),
		SQLITE3_COLUMN_DECLTYPE16(26, "sqlite3_column_decltype16"), SQLITE3_COLUMN_DOUBLE(27, "sqlite3_column_double"),
		SQLITE3_COLUMN_INT(28, "sqlite3_column_int"), SQLITE3_COLUMN_INT64(29, "sqlite3_column_int64"),
		SQLITE3_COLUMN_NAME(30, "sqlite3_column_name"), SQLITE3_COLUMN_NAME16(31, "sqlite3_column_name16"),
		SQLITE3_COLUMN_ORIGIN_NAME(32, "sqlite3_column_origin_name"),
		SQLITE3_COLUMN_ORIGIN_NAME16(33, "sqlite3_column_origin_name16"),
		SQLITE3_COLUMN_TABLE_NAME(34, "sqlite3_column_table_name"),
		SQLITE3_COLUMN_TABLE_NAME16(35, "sqlite3_column_table_name16"), SQLITE3_COLUMN_TEXT(36, "sqlite3_column_text"),
		SQLITE3_COLUMN_TEXT16(37, "sqlite3_column_text16"), SQLITE3_COLUMN_TYPE(38, "sqlite3_column_type"),
		SQLITE3_COLUMN_VALUE(39, "sqlite3_column_value"), SQLITE3_COMMIT_HOOK(40, "sqlite3_commit_hook"),
		SQLITE3_COMPLETE(41, "sqlite3_complete"), SQLITE3_COMPLETE16(42, "sqlite3_complete16"),
		SQLITE3_CREATE_COLLATION(43, "sqlite3_create_collation"),
		SQLITE3_CREATE_COLLATION16(44, "sqlite3_create_collation16"),
		SQLITE3_CREATE_FUNCTION(45, "sqlite3_create_function"),
		SQLITE3_CREATE_FUNCTION16(46, "sqlite3_create_function16"), SQLITE3_CREATE_MODULE(47, "sqlite3_create_module"),
		SQLITE3_DATA_COUNT(48, "sqlite3_data_count"), SQLITE3_DB_HANDLE(49, "sqlite3_db_handle"),
		SQLITE3_DECLARE_VTAB(50, "sqlite3_declare_vtab"),
		SQLITE3_ENABLE_SHARED_CACHE(51, "sqlite3_enable_shared_cache"), SQLITE3_ERRCODE(52, "sqlite3_errcode"),
		SQLITE3_ERRMSG(53, "sqlite3_errmsg"), SQLITE3_ERRMSG16(54, "sqlite3_errmsg16"),
		SQLITE3_EXEC(55, "sqlite3_exec"), SQLITE3_EXPIRED(56, "sqlite3_expired"),
		SQLITE3_FINALIZE(57, "sqlite3_finalize"), SQLITE3_FREE(58, "sqlite3_free"),
		SQLITE3_FREE_TABLE(59, "sqlite3_free_table"), SQLITE3_GET_AUTOCOMMIT(60, "sqlite3_get_autocommit"),
		SQLITE3_GET_AUXDATA(61, "sqlite3_get_auxdata"), SQLITE3_GET_TABLE(62, "sqlite3_get_table"), NULL(63, "0"),
		SQLITE3_INTERRUPT(64, "sqlite3_interrupt"), SQLITE3_LAST_INSERT_ROWID(65, "sqlite3_last_insert_rowid"),
		SQLITE3_LIBVERSION(66, "sqlite3_libversion"), SQLITE3_LIBVERSION_NUMBER(67, "sqlite3_libversion_number"),
		SQLITE3_MALLOC(68, "sqlite3_malloc"), SQLITE3_MPRINTF(69, "sqlite3_mprintf"), SQLITE3_OPEN(70, "sqlite3_open"),
		SQLITE3_OPEN16(71, "sqlite3_open16"), SQLITE3_PREPARE(72, "sqlite3_prepare"),
		SQLITE3_PREPARE16(73, "sqlite3_prepare16"), SQLITE3_PROFILE(74, "sqlite3_profile"),
		SQLITE3_PROGRESS_HANDLER(75, "sqlite3_progress_handler"), SQLITE3_REALLOC(76, "sqlite3_realloc"),
		SQLITE3_RESET(77, "sqlite3_reset"), SQLITE3_RESULT_BLOB(78, "sqlite3_result_blob"),
		SQLITE3_RESULT_DOUBLE(79, "sqlite3_result_double"), SQLITE3_RESULT_ERROR(80, "sqlite3_result_error"),
		SQLITE3_RESULT_ERROR16(81, "sqlite3_result_error16"), SQLITE3_RESULT_INT(82, "sqlite3_result_int"),
		SQLITE3_RESULT_INT64(83, "sqlite3_result_int64"), SQLITE3_RESULT_NULL(84, "sqlite3_result_null"),
		SQLITE3_RESULT_TEXT(85, "sqlite3_result_text"), SQLITE3_RESULT_TEXT16(86, "sqlite3_result_text16"),
		SQLITE3_RESULT_TEXT16BE(87, "sqlite3_result_text16be"), SQLITE3_RESULT_TEXT16LE(88, "sqlite3_result_text16le"),
		SQLITE3_RESULT_VALUE(89, "sqlite3_result_value"), SQLITE3_ROLLBACK_HOOK(90, "sqlite3_rollback_hook"),
		SQLITE3_SET_AUTHORIZER(91, "sqlite3_set_authorizer"), SQLITE3_SET_AUXDATA(92, "sqlite3_set_auxdata"),
		SQLITE3_SNPRINTF(93, "sqlite3_snprintf"), SQLITE3_STEP(94, "sqlite3_step"),
		SQLITE3_TABLE_COLUMN_METADATA(95, "sqlite3_table_column_metadata"),
		SQLITE3_THREAD_CLEANUP(96, "sqlite3_thread_cleanup"), SQLITE3_TOTAL_CHANGES(97, "sqlite3_total_changes"),
		SQLITE3_TRACE(98, "sqlite3_trace"), SQLITE3_TRANSFER_BINDINGS(99, "sqlite3_transfer_bindings"),
		SQLITE3_UPDATE_HOOK(100, "sqlite3_update_hook"), SQLITE3_USER_DATA(101, "sqlite3_user_data"),
		SQLITE3_VALUE_BLOB(102, "sqlite3_value_blob"), SQLITE3_VALUE_BYTES(103, "sqlite3_value_bytes"),
		SQLITE3_VALUE_BYTES16(104, "sqlite3_value_bytes16"), SQLITE3_VALUE_DOUBLE(105, "sqlite3_value_double"),
		SQLITE3_VALUE_INT(106, "sqlite3_value_int"), SQLITE3_VALUE_INT64(107, "sqlite3_value_int64"),
		SQLITE3_VALUE_NUMERIC_TYPE(108, "sqlite3_value_numeric_type"), SQLITE3_VALUE_TEXT(109, "sqlite3_value_text"),
		SQLITE3_VALUE_TEXT16(110, "sqlite3_value_text16"), SQLITE3_VALUE_TEXT16BE(111, "sqlite3_value_text16be"),
		SQLITE3_VALUE_TEXT16LE(112, "sqlite3_value_text16le"), SQLITE3_VALUE_TYPE(113, "sqlite3_value_type"),
		SQLITE3_VMPRINTF(114, "sqlite3_vmprintf"), SQLITE3_OVERLOAD_FUNCTION(115, "sqlite3_overload_function"),
		SQLITE3_PREPARE_V2(116, "sqlite3_prepare_v2"), SQLITE3_PREPARE16_V2(117, "sqlite3_prepare16_v2"),
		SQLITE3_CLEAR_BINDINGS(118, "sqlite3_clear_bindings"),
		SQLITE3_CREATE_MODULE_V2(119, "sqlite3_create_module_v2"), SQLITE3_BIND_ZEROBLOB(120, "sqlite3_bind_zeroblob"),
		SQLITE3_BLOB_BYTES(121, "sqlite3_blob_bytes"), SQLITE3_BLOB_CLOSE(122, "sqlite3_blob_close"),
		SQLITE3_BLOB_OPEN(123, "sqlite3_blob_open"), SQLITE3_BLOB_READ(124, "sqlite3_blob_read"),
		SQLITE3_BLOB_WRITE(125, "sqlite3_blob_write"), SQLITE3_CREATE_COLLATION_V2(126, "sqlite3_create_collation_v2"),
		SQLITE3_FILE_CONTROL(127, "sqlite3_file_control"), SQLITE3_MEMORY_HIGHWATER(128, "sqlite3_memory_highwater"),
		SQLITE3_MEMORY_USED(129, "sqlite3_memory_used"), SQLITE3_MUTEX_ALLOC(130, "sqlite3_mutex_alloc"),
		SQLITE3_MUTEX_ENTER(131, "sqlite3_mutex_enter"), SQLITE3_MUTEX_FREE(132, "sqlite3_mutex_free"),
		SQLITE3_MUTEX_LEAVE(133, "sqlite3_mutex_leave"), SQLITE3_MUTEX_TRY(134, "sqlite3_mutex_try"),
		SQLITE3_OPEN_V2(135, "sqlite3_open_v2"), SQLITE3_RELEASE_MEMORY(136, "sqlite3_release_memory"),
		SQLITE3_RESULT_ERROR_NOMEM(137, "sqlite3_result_error_nomem"),
		SQLITE3_RESULT_ERROR_TOOBIG(138, "sqlite3_result_error_toobig"), SQLITE3_SLEEP(139, "sqlite3_sleep"),
		SQLITE3_SOFT_HEAP_LIMIT(140, "sqlite3_soft_heap_limit"), SQLITE3_VFS_FIND(141, "sqlite3_vfs_find"),
		SQLITE3_VFS_REGISTER(142, "sqlite3_vfs_register"), SQLITE3_VFS_UNREGISTER(143, "sqlite3_vfs_unregister"),
		SQLITE3_THREADSAFE(144, "sqlite3_threadsafe"), SQLITE3_RESULT_ZEROBLOB(145, "sqlite3_result_zeroblob"),
		SQLITE3_RESULT_ERROR_CODE(146, "sqlite3_result_error_code"), SQLITE3_TEST_CONTROL(147, "sqlite3_test_control"),
		SQLITE3_RANDOMNESS(148, "sqlite3_randomness"), SQLITE3_CONTEXT_DB_HANDLE(149, "sqlite3_context_db_handle"),
		SQLITE3_EXTENDED_RESULT_CODES(150, "sqlite3_extended_result_codes"), SQLITE3_LIMIT(151, "sqlite3_limit"),
		SQLITE3_NEXT_STMT(152, "sqlite3_next_stmt"), SQLITE3_SQL(153, "sqlite3_sql"),
		SQLITE3_STATUS(154, "sqlite3_status"), SQLITE3_BACKUP_FINISH(155, "sqlite3_backup_finish"),
		SQLITE3_BACKUP_INIT(156, "sqlite3_backup_init"), SQLITE3_BACKUP_PAGECOUNT(157, "sqlite3_backup_pagecount"),
		SQLITE3_BACKUP_REMAINING(158, "sqlite3_backup_remaining"), SQLITE3_BACKUP_STEP(159, "sqlite3_backup_step"),
		SQLITE3_COMPILEOPTION_GET(160, "sqlite3_compileoption_get"),
		SQLITE3_COMPILEOPTION_USED(161, "sqlite3_compileoption_used"),
		SQLITE3_CREATE_FUNCTION_V2(162, "sqlite3_create_function_v2"), SQLITE3_DB_CONFIG(163, "sqlite3_db_config"),
		SQLITE3_DB_MUTEX(164, "sqlite3_db_mutex"), SQLITE3_DB_STATUS(165, "sqlite3_db_status"),
		SQLITE3_EXTENDED_ERRCODE(166, "sqlite3_extended_errcode"), SQLITE3_LOG(167, "sqlite3_log"),
		SQLITE3_SOFT_HEAP_LIMIT64(168, "sqlite3_soft_heap_limit64"), SQLITE3_SOURCEID(169, "sqlite3_sourceid"),
		SQLITE3_STMT_STATUS(170, "sqlite3_stmt_status"), SQLITE3_STRNICMP(171, "sqlite3_strnicmp"),
		SQLITE3_UNLOCK_NOTIFY(172, "sqlite3_unlock_notify"),
		SQLITE3_WAL_AUTOCHECKPOINT(173, "sqlite3_wal_autocheckpoint"),
		SQLITE3_WAL_CHECKPOINT(174, "sqlite3_wal_checkpoint"), SQLITE3_WAL_HOOK(175, "sqlite3_wal_hook"),
		SQLITE3_BLOB_REOPEN(176, "sqlite3_blob_reopen"), SQLITE3_VTAB_CONFIG(177, "sqlite3_vtab_config"),
		SQLITE3_VTAB_ON_CONFLICT(178, "sqlite3_vtab_on_conflict"), SQLITE3_CLOSE_V2(179, "sqlite3_close_v2"),
		SQLITE3_DB_FILENAME(180, "sqlite3_db_filename"), SQLITE3_DB_READONLY(181, "sqlite3_db_readonly"),
		SQLITE3_DB_RELEASE_MEMORY(182, "sqlite3_db_release_memory"), SQLITE3_ERRSTR(183, "sqlite3_errstr"),
		SQLITE3_STMT_BUSY(184, "sqlite3_stmt_busy"), SQLITE3_STMT_READONLY(185, "sqlite3_stmt_readonly"),
		SQLITE3_STRICMP(186, "sqlite3_stricmp"), SQLITE3_URI_BOOLEAN(187, "sqlite3_uri_boolean"),
		SQLITE3_URI_INT64(188, "sqlite3_uri_int64"), SQLITE3_URI_PARAMETER(189, "sqlite3_uri_parameter"),
		SQLITE3_VSNPRINTF(190, "sqlite3_vsnprintf"), SQLITE3_WAL_CHECKPOINT_V2(191, "sqlite3_wal_checkpoint_v2"),
		SQLITE3_AUTO_EXTENSION(192, "sqlite3_auto_extension"), SQLITE3_BIND_BLOB64(193, "sqlite3_bind_blob64"),
		SQLITE3_BIND_TEXT64(194, "sqlite3_bind_text64"),
		SQLITE3_CANCEL_AUTO_EXTENSION(195, "sqlite3_cancel_auto_extension"),
		SQLITE3_LOAD_EXTENSION(196, "sqlite3_load_extension"), SQLITE3_MALLOC64(197, "sqlite3_malloc64"),
		SQLITE3_MSIZE(198, "sqlite3_msize"), SQLITE3_REALLOC64(199, "sqlite3_realloc64"),
		SQLITE3_RESET_AUTO_EXTENSION(200, "sqlite3_reset_auto_extension"),
		SQLITE3_RESULT_BLOB64(201, "sqlite3_result_blob64"), SQLITE3_RESULT_TEXT64(202, "sqlite3_result_text64"),
		SQLITE3_STRGLOB(203, "sqlite3_strglob"), SQLITE3_VALUE_DUP(204, "sqlite3_value_dup"),
		SQLITE3_VALUE_FREE(205, "sqlite3_value_free"), SQLITE3_RESULT_ZEROBLOB64(206, "sqlite3_result_zeroblob64"),
		SQLITE3_BIND_ZEROBLOB64(207, "sqlite3_bind_zeroblob64"), SQLITE3_VALUE_SUBTYPE(208, "sqlite3_value_subtype"),
		SQLITE3_RESULT_SUBTYPE(209, "sqlite3_result_subtype"), SQLITE3_STATUS64(210, "sqlite3_status64"),
		SQLITE3_STRLIKE(211, "sqlite3_strlike"), SQLITE3_DB_CACHEFLUSH(212, "sqlite3_db_cacheflush"),
		SQLITE3_SYSTEM_ERRNO(213, "sqlite3_system_errno"), SQLITE3_TRACE_V2(214, "sqlite3_trace_v2"),
		SQLITE3_EXPANDED_SQL(215, "sqlite3_expanded_sql"),
		SQLITE3_SET_LAST_INSERT_ROWID(216, "sqlite3_set_last_insert_rowid"),
		SQLITE3_PREPARE_V3(217, "sqlite3_prepare_v3"), SQLITE3_PREPARE16_V3(218, "sqlite3_prepare16_v3"),
		SQLITE3_BIND_POINTER(219, "sqlite3_bind_pointer"), SQLITE3_RESULT_POINTER(220, "sqlite3_result_pointer"),
		SQLITE3_VALUE_POINTER(221, "sqlite3_value_pointer"), SQLITE3_VTAB_NOCHANGE(222, "sqlite3_vtab_nochange"),
		SQLITE3_VALUE_NOCHANGE(223, "sqlite3_value_nochange"), SQLITE3_VTAB_COLLATION(224, "sqlite3_vtab_collation"),
		SQLITE3_KEYWORD_COUNT(225, "sqlite3_keyword_count"), SQLITE3_KEYWORD_NAME(226, "sqlite3_keyword_name"),
		SQLITE3_KEYWORD_CHECK(227, "sqlite3_keyword_check"), SQLITE3_STR_NEW(228, "sqlite3_str_new"),
		SQLITE3_STR_FINISH(229, "sqlite3_str_finish"), SQLITE3_STR_APPENDF(230, "sqlite3_str_appendf"),
		SQLITE3_STR_VAPPENDF(231, "sqlite3_str_vappendf"), SQLITE3_STR_APPEND(232, "sqlite3_str_append"),
		SQLITE3_STR_APPENDALL(233, "sqlite3_str_appendall"), SQLITE3_STR_APPENDCHAR(234, "sqlite3_str_appendchar"),
		SQLITE3_STR_RESET(235, "sqlite3_str_reset"), SQLITE3_STR_ERRCODE(236, "sqlite3_str_errcode"),
		SQLITE3_STR_LENGTH(237, "sqlite3_str_length"), SQLITE3_STR_VALUE(238, "sqlite3_str_value"),
		SQLITE3_CREATE_WINDOW_FUNCTION(239, "sqlite3_create_window_function"),
		SQLITE3_NORMALIZED_SQL(240, "sqlite3_normalized_sql"), SQLITE3_STMT_ISEXPLAIN(241, "sqlite3_stmt_isexplain"),
		SQLITE3_VALUE_FROMBIND(242, "sqlite3_value_frombind"), SQLITE3_DROP_MODULES(243, "sqlite3_drop_modules"),
		SQLITE3_HARD_HEAP_LIMIT64(244, "sqlite3_hard_heap_limit64"), SQLITE3_URI_KEY(245, "sqlite3_uri_key"),
		SQLITE3_FILENAME_DATABASE(246, "sqlite3_filename_database"),
		SQLITE3_FILENAME_JOURNAL(247, "sqlite3_filename_journal"), SQLITE3_FILENAME_WAL(248, "sqlite3_filename_wal"),
		SQLITE3_CREATE_FILENAME(249, "sqlite3_create_filename"), SQLITE3_FREE_FILENAME(250, "sqlite3_free_filename"),
		SQLITE3_DATABASE_FILE_OBJECT(251, "sqlite3_database_file_object"), SQLITE3_TXN_STATE(252, "sqlite3_txn_state"),
		SQLITE3_CHANGES64(253, "sqlite3_changes64"), SQLITE3_TOTAL_CHANGES64(254, "sqlite3_total_changes64"),
		SQLITE3_AUTOVACUUM_PAGES(255, "sqlite3_autovacuum_pages"), SQLITE3_ERROR_OFFSET(256, "sqlite3_error_offset"),
		SQLITE3_VTAB_RHS_VALUE(257, "sqlite3_vtab_rhs_value"), SQLITE3_VTAB_DISTINCT(258, "sqlite3_vtab_distinct"),
		SQLITE3_VTAB_IN(259, "sqlite3_vtab_in"), SQLITE3_VTAB_IN_FIRST(260, "sqlite3_vtab_in_first"),
		SQLITE3_VTAB_IN_NEXT(261, "sqlite3_vtab_in_next"), SQLITE3_DESERIALIZE(262, "sqlite3_deserialize"),
		SQLITE3_SERIALIZE(263, "sqlite3_serialize"), SQLITE3_DB_NAME(264, "sqlite3_db_name"),
		SQLITE3_VALUE_TYPE_3_40(265, "sqlite3_value_type");

		private EnumSql(Integer code, String desc) {
			this.code = code;
			this.desc = desc;
		}

		private Integer code;
		private String desc;

		public Integer getCode() {
			return code;
		}

		public void setCode(Integer code) {
			this.code = code;
		}

		public String getDesc() {
			return desc;
		}

		public void setDesc(String desc) {
			this.desc = desc;
		}

		public static EnumSql getByCode(Integer code) {
			for (EnumSql item : EnumSql.values()) {
				if (item.getCode().equals(code)) {
					return item;
				}
			}
			return null;
		}

		public static EnumSql getByCode(String desc) {
			for (EnumSql item : EnumSql.values()) {
				if (item.getDesc().equals(desc)) {
					return item;
				}
			}
			return null;
		}

	}

}
