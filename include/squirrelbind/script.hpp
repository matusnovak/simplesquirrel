#pragma once
#ifndef SQUIRREL_BIND_SCRIPT_HEADER_H
#define SQUIRREL_BIND_SCRIPT_HEADER_H

namespace SquirrelBind {
	class SqScript: public SqObject {
	public:
		SqScript(HSQUIRRELVM vm);
		virtual ~SqScript();
		/**
		* @brief Swaps two objects
		*/
		void swap(SqScript& other) NOEXCEPT;
		/**
		* @brief Deleted copy constructor
		*/
		SqScript(const SqScript& other) = delete;
		/**
		* @brief Move constructor
		*/
		SqScript(SqScript&& other) NOEXCEPT;
		/**
		* @brief Deleted copy assignment operator
		*/
		SqScript& operator = (const SqScript& other) = delete;
		/**
		* @brief Move assignment operator
		*/
		SqScript& operator = (SqScript&& other) NOEXCEPT;
	};
}

#endif